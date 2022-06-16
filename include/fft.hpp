#include <complex>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <thread>

#include <iostream>
//std::vector<std::complex<double>> radix2fft(std::vector<double>);
void radix2fft(std::vector<double> &P, size_t n, size_t num_threads, std::vector<std::complex<double>> &res);
void inv_radix2fft(std::vector<std::complex<double>> &P, size_t n, size_t num_threads, std::vector<double> &res);
std::vector<double> fft_poly_mult(double *p1, size_t n1, double *p2, size_t n2, size_t num_threads);
