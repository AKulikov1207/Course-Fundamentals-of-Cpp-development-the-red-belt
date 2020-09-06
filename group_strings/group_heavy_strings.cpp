#include <algorithm>
#include <string>
#include <vector>
#include <map>

#include "test_runner.h"

using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
template <typename String>
using Group = vector<String>;

// вместо typename String::value_type
// используем Char<String>
template <typename String>
using Char = typename String::value_type;

template<typename String>
using Key = String;

template<typename String>
Key<String> ComputeKeyString(const String& str) {
	String chars = str;

	sort(chars.begin(), chars.end());
	chars.erase(unique(chars.begin(), chars.end()), chars.end());

	return chars;
}

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
	map<Key<String>, Group<String>> map_groups;

	for (String& str : strings)
		map_groups[ComputeKeyString(str)].push_back(move(str));

	vector<Group<String>> vec_groups;

	for (auto& [key, group] : map_groups)
		vec_groups.push_back(move(group));

	return vec_groups;
}


void TestGroupingABC() {
  vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2);
  sort(begin(groups), end(groups));  // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
  ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
  vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4);
  sort(begin(groups), end(groups));  // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
  ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
  ASSERT_EQUAL(groups[2], vector<string>({"port"}));
  ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);
  return 0;
}
