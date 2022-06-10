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
	 bool stopped;
};

template <class E>
void SafeUnboundedQueue<E>::push(const E& element) {
	std::lock_guard<std::mutex> guard(lock);
	elements.push(element);
	not_empty.notify_all();
}

template <class E> 
E SafeUnboundedQueue<E>::pop() {
	std::unique_lock<std::mutex> lk(lock);
	while (is_empty()) not_empty.wait(lk);
	E tmp = elements.front();
	elements.pop();
	return tmp;
}

//-----------------------------------------------------------------------------


void SimplePool::do_work() {
	while (tasks.pop()());
}

SimplePool::SimplePool(unsigned int num_workers) {
	this->num_workers = num_workers;
	for (unsigned int i = 0; i < num_workers; i++) {
		workers.push_back(std::thread(&SimplePool::do_work, this));
	}
	stopped = false;
}

SimplePool::~SimplePool() {
	 if (!stopped) stop();
}

template <class F, class... Args>
void SimplePool::push(F f, Args ... args) {
	tasks.push([f,args...]() -> bool { f(args...); return true; });
}

void SimplePool::stop() {
	for (unsigned int i = 0; i < num_workers; i++)
		tasks.push([]() -> bool { return false; });
	for (unsigned int i = 0; i < num_workers; i++)
		workers[i].join();
	stopped = true;
}

