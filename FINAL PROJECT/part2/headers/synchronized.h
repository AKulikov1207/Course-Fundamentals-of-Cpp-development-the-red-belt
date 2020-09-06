#pragma once

#include <mutex>

using namespace std;

template <typename T>
class Synchronized {
public:
	explicit Synchronized(T initial = T()) : value(move(initial)) {}

	struct Access {
		lock_guard<mutex> guard;
		T& ref_to_value;
	};

	Access GetAccess() {
		return { lock_guard<mutex>(m), value };
	}

private:
	T value;
	mutex m;
};