#include <iostream>
#include "threadpool.hpp"
#include "fft.hpp"

void increment(int *i) { *i += 1; }

int main() {
	int i;
	double p1[7] = {1, 1, 4, 5, 6, 7, 8};
	double p2[8] = {1, -1, 6, -7, 2, 1, 0, -9};
	std::vector<double> p3 = fft_poly_mult(p1, 7, p2, 8, 1);

	auto begin = p3.begin();
	auto end = p3.end();
	i = 0;
	while (begin != end) {
		std::cout << *begin << "x^" << i << " + ";
		i++;
		begin++;
	}
	std::cout << std::endl;

	double p4[4] = {5,3,2,1};
	std::vector<double> p4v(4);
	std::vector<std::complex<double>> p5(4);
	for (int i = 0; i < 4; i++) {
		p4v[i] = p4[i];
	}
	radix2fft(p4v, 4, 1, p5);
	 
	auto begin_2 = p5.begin();
	auto end_2 = p5.end();
	i = 0;
	while (begin_2 != end_2) {
		std::cout << *begin_2 << std::endl;
		i++;
		begin_2++;
	}

	std::vector<std::complex<double>> res(4); 
	inv_radix2fft(p5, 4, 1, res);
	auto begin_3 = res.begin();
	auto end_3 = res.end();
	i = 0;
	while (begin_3 != end_3) {
		std::cout << *begin_3 << std::endl;
		i++;
		begin_3++;
	}

	return 0;
}
