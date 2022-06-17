#include <complex>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <thread>
#include <iostream>

typedef std::vector<std::complex<double>> ComVector;

//std::vector<std::complex<double>> radix2fft(std::vector<double>);
void radix2fft(std::vector<double> &P, size_t n, size_t num_threads, ComVector &res);

void inv_radix2fft(ComVector &P, size_t n, size_t num_threads, ComVector &res);

std::vector<double> fft_poly_mult(double *p1, size_t n1, double *p2, size_t n2, size_t num_threads);

void trim_less_than(ComVector &P, double cutoff);

void trim_signal(ComVector &P, size_t low, size_t high);

//void trim_n_perc_of_max(ComVector &P, double perc);
