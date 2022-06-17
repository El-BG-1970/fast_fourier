#include <iostream>
#include "threadpool.hpp"
#include "fft.hpp"
#include "csv_utils.hpp"
#define testFFSampling

void increment(int *i) { *i += 1; }

int main() {
#ifdef testPolyMult
	size_t i;
	double p1[7] = {1, 1, 4, 5, 6, 7, 8};
	double p2[8] = {1, -1, 6, -7, 2, 1, 0, -9};
	std::vector<double> p3 = fft_poly_mult(p1, 7, p2, 8, 7);

	i = 0;
	while (i < p3.size()-1) {
		std::cout << p3[i] << "x^" << i << " + ";
		i++;
	}
	std::cout << p3[i] << "x^" << i << std::endl;
#endif

#ifdef testFFT
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
#endif

#ifdef testFFSampling
    std::vector<double> *data = read_data("./data/date_v_temp.csv");
    size_t ds = data->size();
    size_t sz = 1;
    while (sz < ds) sz = sz << 1;

    for (size_t i = ds; i < sz; i++) data->push_back(0.);
    std::vector<std::complex<double>> samples(sz);
    radix2fft(std::ref(*data), sz, 2, std::ref(samples));

    // here we let go of some coefficients (second parameter is percent)
    trim_less_than(std::ref(samples), 0.9);
    
    // here we need to do the reverse fft in order to obtain the compressed data
    std::vector<std::complex<double>> ret_c(sz, 0.);
    inv_radix2fft(std::ref(samples), sz, 2, std::ref(ret_c));
    std::vector<double> ret_c_double(sz, 0.);
    for (size_t i = 0; i < ds; i++)
        ret_c_double[i] = std::real(ret_c[i]);
    
    // finally, we need to write a csv with the original data and the compressed data, for comparison
    write_data("./data/date_v_temp_FFT.csv", std::ref(ret_c_double), ds);
#endif

	return 0;
}
