#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>

#include "test_runner.h"

using namespace std;

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
	const uint32_t range_size = last - first;

	if (range_size <= 2)
		return;

	vector<uint32_t> next_pos(range_size);
	iota(next_pos.begin(), next_pos.end(), 1);
	next_pos.back() = 0;
	vector<typename RandomIt::value_type> permut;
	uint32_t cur_pos = 0;
	uint32_t prev_alive_pos = range_size - 1;

	for (uint32_t step = 0; step < range_size; ++step) {
		permut.push_back(move(*(first + cur_pos)));
		next_pos[prev_alive_pos] = next_pos[cur_pos];

		for (uint32_t i = 0; i < step_size; ++i) {
			if (i > 0) {
				prev_alive_pos = cur_pos;
			}
			cur_pos = next_pos[cur_pos];
		}
	}

	for (uint32_t i = 0; i < range_size; ++i, ++first)
		*first = move(permut[i]);
}

vector<int> MakeTestVector() {
	vector<int> numbers(10);
	iota(begin(numbers), end(numbers), 0);
	return numbers;
}

void TestIntVector() {
  const vector<int> numbers = MakeTestVector();
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    ASSERT_EQUAL(numbers_copy, numbers);
  }
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
    ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
  }
}

// Это специальный тип, который поможет убедиться, что реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.

struct NoncopyableInt {
  int value;

  NoncopyableInt(const NoncopyableInt&) = delete;
  NoncopyableInt& operator=(const NoncopyableInt&) = delete;

  NoncopyableInt(NoncopyableInt&&) = default;
  NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
  return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
  return os << v.value;
}

void TestAvoidsCopying() {
  vector<NoncopyableInt> numbers;
  numbers.push_back({1});
  numbers.push_back({2});
  numbers.push_back({3});
  numbers.push_back({4});
  numbers.push_back({5});

  MakeJosephusPermutation(begin(numbers), end(numbers), 2);

  vector<NoncopyableInt> expected;
  expected.push_back({1});
  expected.push_back({3});
  expected.push_back({5});
  expected.push_back({4});
  expected.push_back({2});

  ASSERT_EQUAL(numbers, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  RUN_TEST(tr, TestAvoidsCopying);
  return 0;
}
