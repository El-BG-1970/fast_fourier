#include <iostream>
#include "threadpool.hpp"
#include "fft.hpp"

void increment(int *i) { *i += 1; }

int main() {
	 double p1[7] = {1, 1, 4, 5, 6, 7, 8};
	 double p2[8] = {1, -1, 6, -7, 2, 1, 0, -9};
	 std::vector<double> p3 = fft_poly_mult(p1, 3, p2, 3, 1);

	auto begin = p3.begin();
	auto end = p3.end();
	int i = 0;
	while (begin != end) {
		std::cout << *begin << "x^" << i << " + ";
		i++;
		begin++;
	}
	std::cout << std::endl;
	 
	 return 0;
}
