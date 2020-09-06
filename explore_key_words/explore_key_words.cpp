#include "test_runner.h"
#include "profile.h"

#include <map>
#include <set>
#include <string>
#include <future>
#include <vector>

using namespace std;

struct Stats {
	map<string, int> word_frequences;

	void operator += (const Stats& other) {
		for (auto& s : other.word_frequences)
			word_frequences[s.first] += s.second;
	}
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
	string tmp = string();

	Stats stats;

	for (const char& ch : line) {
		if (ch == ' ') {
			if (!tmp.empty() && key_words.find(tmp) != key_words.end())
				++stats.word_frequences[move(tmp)];
			tmp = string();
		}
		else
			tmp.push_back(ch);
	}

	if (!tmp.empty() && key_words.find(tmp) != key_words.end()) {
		++stats.word_frequences[move(tmp)];
	}

	return stats;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
	Stats result;

	for (string line; getline(input, line);) {
		result += ExploreLine(key_words, line);
	}

	return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
	Stats res;
	vector<string> strs;
	string s;

	while (getline(input, s)) {
		strs.push_back(move(s));
	}

	vector<stringstream> streams;
	streams.reserve(4);

	for (size_t i = 0; i < 4; ++i) {
		stringstream ss;

		for (auto it = strs.begin() + strs.size() * i / 4; it != strs.end() &&
			it != strs.begin() + strs.size() * (i + 1) / 4; ++it) {
			ss << *it << '\n';
		}
		streams.emplace_back(move(ss));
	}

	vector<future<Stats>> f;
	f.reserve(4);

	for (auto& s : streams) {
		f.emplace_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(s)));
	}

	for (auto& t : f)
		res += t.get();

	return res;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
