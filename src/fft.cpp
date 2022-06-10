#include <fft.hpp>

using namespace std::complex_literals;
std::complex<double> *radix2fft(double *P, size_t n) {
	 if (n == 1) {
		  std::complex<double> *ret = (std::complex<double>*) malloc(sizeof(std::complex<double>));
		  ret[0] = P[0];
		  return ret;
	 }

	 size_t half_n = (n/2) - 1;
	 
	 std::complex<double> *ret;
	 double *U, *V;
	 ret = (std::complex<double>*) malloc(n*sizeof(std::complex<double>));
	 U = (double*) malloc(half_n*sizeof(double));
	 V = (double*) malloc(half_n*sizeof(double));

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
	 
	 free(U);
	 free(V);
	 return ret;
}

double *inv_radix2fft(std::complex<double> *P, size_t n) {
	 if (n == 1) {
		  double *ret = (double*) malloc(sizeof(double));
		  ret[0] = std::real(P[0]);
		  return ret;
	 }

	 size_t half_n = (n/2) - 1;
	 
	 double *ret;
	 std::complex<double> *U, *V;
	 ret = (double*) malloc(n*sizeof(double));
	 U = (std::complex<double>*) malloc(half_n*sizeof(std::complex<double>));
	 V = (std::complex<double>*) malloc(half_n*sizeof(std::complex<double>));

	 for (size_t j = 0; j < half_n; j++) {
		  U[j] = P[2*j];
		  V[j] = P[2*j+1];
	 }

	 double *Ut = inv_radix2fft(U, half_n);
	 double *Vt = inv_radix2fft(V, half_n);
	 
	 std::complex<double> om_n = std::exp(-(2/n)*M_PI*1i);
	 std::complex<double> om = 1;

	 for (size_t j = 0; j < half_n; j++) {
		  ret[j] = std::real(Ut[j] + om*Vt[j])/2;
		  ret[j+(n/2)] = std::real(Ut[j] - om*Vt[j])/2;
		  om = om * om_n;
	 }
	 
	 free(U);
	 free(V);
	 return ret;
}

double *fft_poly_mult(double *p1, size_t n1, double *p2, size_t n2) {
	 size_t sum = (n1+n2-1);
	 size_t total = 1;
	 while (total < sum) total = total << 1;
	 
	 double *P = (double *)calloc(total, sizeof(double));
	 double *Q = (double *)calloc(total, sizeof(double));
	 std::complex<double> *tmp = (std::complex<double> *)calloc(total, sizeof(std::complex<double>));

	 for (size_t i = 0; i < n1; P[i] = p1[i++]);
	 for (size_t i = 0; i < n2; Q[i] = p2[i++]);

	 std::complex<double> *Pt = radix2fft(P, total);
	 std::complex<double> *Qt = radix2fft(Q, total);
	 free(P); free(Q);
	 
	 for (size_t j = 0; j < total; j++) 
		  tmp[j] = Pt[j] * Qt[j];

	 double *ret = inv_radix2fft(tmp, total);
	 free(tmp);
	 return ret;
}
