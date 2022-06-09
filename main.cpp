#include <iostream>
#include <threadpool.hpp>

int main(int argc, char **argv) {
	 SimplePool threadpool(2);
	 
	 std::cout << "hello world" << std::endl;
	 return 0;
}
