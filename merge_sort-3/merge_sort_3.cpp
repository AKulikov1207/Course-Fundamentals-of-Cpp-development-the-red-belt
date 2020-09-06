#include <algorithm>
#include <memory>
#include <vector>

#include "test_runner.h"

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
	// 1. Если диапазон содержит меньше 2 элементов, выходим из функции
	if (range_end - range_begin < 2)
		return;

	// 2. Создаем вектор, содержащий все элементы текущего диапазона
	vector<typename RandomIt::value_type> elements(make_move_iterator(range_begin), make_move_iterator(range_end));

	// 3. Разбиваем вектор на три равные части
	auto it = elements.begin() + elements.size() / 3;
	auto it2 = it + elements.size() / 3;

	// 4. Вызываем функцию MergeSort от каждой трети вектора
	MergeSort(elements.begin(), it);
	MergeSort(it, it2);
	MergeSort(it2, elements.end());

	// 5. С помощью алгоритма merge cливаем первые две трети во временный вектор
	vector<typename RandomIt::value_type> tmp;
	merge(make_move_iterator(elements.begin()), make_move_iterator(it), make_move_iterator(it), make_move_iterator(it2), back_inserter(tmp));

	// 6. С помощью алгоритма merge сливаем отсортированные части
	merge(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()), make_move_iterator(it2), make_move_iterator(elements.end()), range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
