#pragma once

#include <cstdlib>
#include <algorithm>

template <typename T>
class SimpleVector {
public:
	SimpleVector() = default;

	explicit SimpleVector(size_t size) : data{ new T[size] }, size_{ size }, capacity{ size } {}

	SimpleVector(const SimpleVector& other) : data{ new T[other.capacity] }, size_{ other.size_ }, capacity{ other.capacity } {
		std::copy(other.begin(), other.end(), begin());
	}

	~SimpleVector() {
		delete[] data;
	}

	void operator=(const SimpleVector& other) {
		if (other.size_ <= capacity) {
			std::copy(other.begin(), other.end(), begin());
			size_ = other.size_;
		}
		else {
			SimpleVector<T> tmp(other);

			std::swap(tmp.data, data);
			std::swap(tmp.size_, size_);
			std::swap(tmp.capacity, capacity);
		}
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

	const T* begin() const {
		return data;
	}

	const T* end() const {
		return data + size_;
	}

	size_t Size() const {
		return size_;
	}

	size_t Capacity() const {
		return capacity;
	}

	void PushBack(const T& value) {
		if (size_ == 0) {
			capacity = 1;
			++size_;
			delete[] data;
			data = new T[1];
			data[0] = value;
		}
		else if (size_ != capacity) {
			data[size_] = value;
			++size_;
		}
		else {
			capacity *= 2;
			T* tmp = new T[size_];

			for (size_t i = 0; i < size_; ++i) {
				tmp[i] = data[i];
			}

			delete[] data;
			data = new T[capacity];

			for (size_t i = 0; i < size_; ++i) {
				data[i] = tmp[i];
			}

			data[size_] = value;
			++size_;

			delete[] tmp;
		}
	}

private:
	T* data = nullptr;
	size_t size_ = 0;
	size_t capacity = 0;
};
