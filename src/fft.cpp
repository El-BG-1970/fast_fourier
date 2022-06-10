#include <fft.hpp>

using namespace std::complex_literals;
std::complex<double> *radix2fft(std::complex<double> *P, size_t n) {
	 if (n == 1) return P;

	 size_t half_n = (n/2) - 1;
	 
	 std::complex<double> *ret, *U, *V;
	 ret = (std::complex<double>*) malloc(n*sizeof(std::complex<double>));
	 U = (std::complex<double>*) malloc(half_n*sizeof(std::complex<double>));
	 V = (std::complex<double>*) malloc(half_n*sizeof(std::complex<double>));

	 for (size_t j = 0; j < half_n; j++) {
		  U[j] = P[2*j];
		  V[j] = P[2*j+1];
	 }

	 std::complex<double> *Ut = radix2fft(U, half_n);
	 std::complex<double> *Vt = radix2fft(V, half_n);
	 
	 std::complex<double> om_n = std::exp((2/n)*M_PI*1i);
	 std::complex<double> om = 1;

	 for (size_t j = 0; j < half_n; j++) {
		  ret[j] = Ut[j] + om*Vt[j];
		  ret[j+(n/2)] = Ut[j] - om*Vt[j];
		  om = om * om_n;
	 }
	 
	 return ret;
}
