#include <cstdint>
#include <algorithm>

using namespace std;

template <typename T>
class SimpleVector {
public:
  SimpleVector() = default;
  explicit SimpleVector(size_t size) : data{ new T[size] }, size_{ size }, capacity{ size } {}
  ~SimpleVector() {
	  delete[] data;
  }

  T& operator[](size_t index) {
	  return data[index];
  }

  T* begin() {
	  return data;
  }

  T* end() {
	  return data + size_;
  }

  size_t Size() const {
	  return size_;
  }

  size_t Capacity() const {
	  return capacity;
  }
  void PushBack(T value) {
	  ExpandIfNeeded();
	  data[size_++] = move(value);
  }

private:
	T* data = nullptr;
	size_t size_ = 0;
	size_t capacity = 0;

	void ExpandIfNeeded() {
		if (size_ >= capacity) {
			auto new_cap = capacity == 0 ? 1 : 2 * capacity;
			auto new_data = new T[new_cap];
			// используем перемещение
			move(begin(), end(), new_data);
			delete[] data;
			data = new_data;
			capacity = new_cap;
		}
	}
};
