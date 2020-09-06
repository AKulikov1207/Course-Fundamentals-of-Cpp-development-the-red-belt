#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <unordered_map>
#include <set>
#include <utility>
#include <vector>

#include "test_runner.h"

using namespace std;

template <typename T>
class PriorityCollection {
public:
	using Id = int;

	// Добавить объект с нулевым приоритетом
	// с помощью перемещения и вернуть его идентификатор
	Id Add(T object) {
		auto it = ids.insert({ 0, count }).first;
		objects[count] = move(object);
		iters[count] = it;
		return count++;
	}

	// Добавить все элементы диапазона [range_begin, range_end)
	// с помощью перемещения, записав выданные им идентификаторы
	// в диапазон [ids_begin, ...)
	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
		for (auto it = range_begin; it != range_end; ++it, ++ids_begin)
			*ids_begin = Add(move(*it));
	}

	// Определить, принадлежит ли идентификатор какому-либо
	// хранящемуся в контейнере объекту
	bool IsValid(Id id) const {
		return objects.count(id) > 0;
	}

	// Получить объект по идентификатору
	const T& Get(Id id) const {
		return objects.at(id);
	}

	// Увеличить приоритет объекта на 1
	void Promote(Id id) {
		auto it = iters[id];
		iters[id] = ids.insert({it->first + 1, it->second}).first;
		ids.erase(it);
	}

	// Получить объект с максимальным приоритетом и его приоритет
	pair<const T&, int> GetMax() const {
		auto it = ids.rbegin();
		return {objects.at(it->second), it->first};
	}

	// Аналогично GetMax, но удаляет элемент из контейнера
	pair<T, int> PopMax() {
		auto it = ids.end();
		--it;
		Id id = (*ids.rbegin()).second;
		pair<T, int> p = make_pair(move(objects[it->second]), it->first);
		ids.erase(*it);
		iters.erase(id);
		objects.erase(id);
		return p;
	}

private:
	set<pair<int, Id>> ids;
	unordered_map<Id, T> objects;
	unordered_map<Id, set<pair<int, Id>>::iterator> iters;
	size_t count = 0;
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
