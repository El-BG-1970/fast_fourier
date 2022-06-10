#include <iostream>
#include "threadpool.hpp"
#include "fft.hpp"

void increment(int *i) { *i += 1; }

int main() {
	 // SimplePool threadpool(2);
	 // int onetwo[2];
	 // onetwo[0] = onetwo[1] = 0;
	 // threadpool.push(&increment, &onetwo[0]);
	 // threadpool.push(&increment, &onetwo[1]);
	 // threadpool.stop();
	 // std::cout << onetwo[0] << ' ' << onetwo[1] << std::endl;
	 double p1[2] = {1, 1};
	 double p2[2] = {1, -1};
	 double *p3 = fft_poly_mult(p1, 2, p2, 2);

	 std::cout << p1[0] << " + " << p1[1] << "x times ";
	 std::cout << p2[0] << " + " << p2[1] << "x equals ";
	 std::cout << p3[0] << " + " << p3[1] << "x + " << p3[2] << "x^2" << std::endl;
	 
	 return 0;
}
