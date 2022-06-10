#include <iostream>
#include "threadpool.hpp"

void increment(int *i) { *i += 1; }

int main() {
	 SimplePool threadpool(2);
	 int onetwo[2];
	 onetwo[0] = onetwo[1] = 0;
	 
	 threadpool.push(&increment, &onetwo[0]);
	 threadpool.push(&increment, &onetwo[1]);
	 threadpool.stop();

	 std::cout << onetwo[0] << ' ' << onetwo[1] << std::endl;
	 return 0;
}
