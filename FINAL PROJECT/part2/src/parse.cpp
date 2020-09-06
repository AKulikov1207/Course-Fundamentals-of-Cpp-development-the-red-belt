#include "parse.h"

string_view Strip(string_view s) {
	while (!s.empty() && isspace(s.front())) {
		s.remove_prefix(1);
	}

	while (!s.empty() && isspace(s.back())) {
		s.remove_suffix(1);
	}

	return s;
}

vector<string_view> SplitBy(string_view s, char sep) {
	vector<string_view> result;

	while (!s.empty()) {
		size_t pos = s.find(sep);
		result.push_back(s.substr(0, pos));
		s.remove_prefix(pos != s.npos ? pos + 1 : s.size());
	}

	return result;
}

void StripLeftWhitespaces(string_view& sv) {
	while (!sv.empty() && isspace(sv.front())) {
		sv.remove_prefix(1);
	}
}

string_view ReadToken(string_view& sv) {
	StripLeftWhitespaces(sv);

	auto pos = sv.find(' ');
	auto result = sv.substr(0, pos);
	sv.remove_prefix(pos != sv.npos ? pos : sv.size());

	return result;
}

vector<string_view> SplitIntoWordsView(string_view line) {
	vector<string_view> result;

	for (string_view word = ReadToken(line); !word.empty(); word = ReadToken(line)) {
		result.push_back(word);
	}

	return result;
}