#ifndef MUTEXQUEUE_HPP
#define MUTEXQUEUE_HPP

#include <queue>
#include <mutex>

template<typename T>
class MutexQueue {
private:
	std::queue<T> queue;
	mutable std::mutex mutex;

public:
	void Push(const T& value) {
		std::lock_guard<std::mutex> lock(mutex);
		queue.push(value);
	}

	T Pop() {
		T front = queue.front();
		queue.pop();
		return front;
	}

	size_t Size() {
		return queue.size();
	}
};



#endif
