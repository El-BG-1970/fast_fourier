#include <complex>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>

//std::vector<std::complex<double>> radix2fft(std::vector<double>);
std::complex<double> *radix2fft(double *P, size_t n);
double *inv_radix2fft(std::complex<double> *P, size_t n);
double *fft_poly_mult(double *p1, size_t n1, double *p2, size_t n2);
