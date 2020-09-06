#include <vector>
#include <future>
#include <numeric>
#include <algorithm>
#include <cstdint>

#include "test_runner.h"

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
	IteratorRange(Iterator begin, Iterator end) : first(begin), last(end), size_(distance(first, last)) {}

	Iterator begin() const { return first; }

	Iterator end() const { return last; }

	size_t size() const { return size_; }

private:
	Iterator first, last;
	size_t size_;
};

template <typename Iterator>
class Paginator {
private:
	vector<IteratorRange<Iterator>> pages;

public:
	Paginator(Iterator begin, Iterator end, size_t page_size) {
		for (size_t left = distance(begin, end); left > 0; ) {
			size_t cur_page_size = min(page_size, left);
			Iterator cur_page_end = next(begin, cur_page_size);
			pages.push_back({ begin, cur_page_end });

			left -= cur_page_size;
			begin = cur_page_end;
		}
	}

	auto begin() const { return pages.begin(); }

	auto end() const { return pages.end(); }

	size_t size() const { return pages.size(); }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
	return Paginator(begin(c), end(c), page_size);
}

template <typename ContainerOfVectors>
int64_t SumSingleThread(const ContainerOfVectors& matrix) {
	int64_t sum = 0;

	for (const auto& row : matrix) {
		for (auto item : row) {
			sum += item;
		}
	}

	return sum;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
	vector<future<int64_t>> futures;

	for (auto p : Paginate(matrix, 2000)) {
		futures.push_back(async([=] { return SumSingleThread(p); }));
	}

	int64_t result(0);
	for (auto& f : futures) {
		result += f.get();
	}

	return result;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
