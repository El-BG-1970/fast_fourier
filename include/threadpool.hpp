#pragma once

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <cmath>
#include <mutex>
#include <numeric>
#include <thread>
#include <queue>


template <class E> 
class SafeUnboundedQueue {
	 std::queue<E> elements;
	 std::mutex lock;
	 std::condition_variable not_empty;
public: 
	 SafeUnboundedQueue<E>(){}
	 void push(const E& element);
	 E pop ();
	 bool is_empty() const {
		  return this->elements.empty();
	 }
};


class SimplePool {
	 unsigned int num_workers;
	 std::vector<std::thread> workers;
	 SafeUnboundedQueue<std::function<bool()>> tasks;
	 void do_work();

public:
	 SimplePool(unsigned int num_workers = 0);
	 ~SimplePool();
	 template <class F, class... Args>
	 void push(F f, Args ... args);
	 void stop();
};
