#include <fft.hpp>

typedef std::vector<std::complex<double>> ComVector;

using namespace std::complex_literals;
void radix2fft(std::vector<double> &P, size_t n, size_t num_threads, ComVector &res) {
	if (n == 1) {
		res[0] = P[0];
		return;
	}
	if (n == 0) {
		return;
	}

	size_t half_n = n/2;
	size_t half_threads1 = num_threads/2;
	size_t half_threads2 = half_threads1 + (num_threads % 2);
	
	std::vector<double> U(half_n);
	std::vector<double> V(half_n);

	for (size_t j = 0; j < half_n; j++) {
		U[j] = P[2*j];
		V[j] = P[2*j+1];
	}

	ComVector Ut(half_n);
	ComVector Vt(half_n);
	// Make one thread run radix2fft and do the other yourself
	if (num_threads == 2) {
		std::thread worker(&radix2fft, std::ref(U), half_n, half_threads1, std::ref(Ut));
		radix2fft(V, half_n, half_threads2, std::ref(Vt));
		worker.join();
	}
	else {
		radix2fft(U, half_n, half_threads1, std::ref(Ut));
		radix2fft(V, half_n, half_threads2, std::ref(Vt));
	}
	
	std::complex<double> coeff(0, M_PI*2/n);
	std::complex<double> omega_1 = std::pow(M_E, coeff);
	std::complex<double> omega = 1;

	for (size_t j = 0; j < half_n; j++) {
		res[j] = Ut[j] + omega*Vt[j];
		res[j+half_n] = Ut[j] - omega*Vt[j];
		omega *= omega_1;
	}
}

void inv_radix2fft(ComVector &P, size_t n, size_t num_threads, std::vector<double> &res) {
	if (n == 1) {
		res[0] = std::real(P[0]);
	}
	if (n == 0) {return;}

	size_t half_n = n/2;
	size_t half_threads1 = num_threads/2;
	size_t half_threads2 = half_threads1 + (num_threads % 2);
	
	ComVector U(half_n);
	ComVector V(half_n);

	for (size_t j = 0; j < half_n; j++) {
		U[j] = P[2*j];
		V[j] = P[2*j+1];
	}

	std::vector<double> Ut(half_n);
	std::vector<double> Vt(half_n);
	inv_radix2fft(U, half_n, half_threads1, Ut);
	inv_radix2fft(V, half_n, half_threads2, Vt);
	
	std::complex<double> coeff(0, M_PI*2/n);
	std::complex<double> omega_1 = std::pow(M_E, coeff);
	std::complex<double> omega = 1;

	for (size_t j = 0; j < half_n; j++) {
		res[j] = std::real(Ut[j] + omega*Vt[j])/2;
		res[j+(n/2)] = std::real(Ut[j] - omega*Vt[j])/2;
		omega *= omega_1;
	}
}

std::vector<double> fft_poly_mult(double *p1, size_t n1, double *p2, size_t n2, size_t num_threads) {
	size_t sum = (n1+n2-1);
	size_t total = 1;
	while (total < sum) total = total << 1;
	
	std::vector<double> P(total);
	std::vector<double> Q(total);
	ComVector tmp(total);

	for (size_t i = 0; i < n1; i++) {P[i] = p1[i];}
	for (size_t i = 0; i < n2; i++) {Q[i] = p2[i];}

	size_t half_threads1 = num_threads/2;
	size_t half_threads2 = half_threads1 + (num_threads % 2);
	ComVector Pt(total);
	ComVector Qt(total);
	radix2fft(P, total, half_threads1, std::ref(Pt));
	radix2fft(Q, total, half_threads2, std::ref(Qt));
	
	for (size_t j = 0; j < total; j++) 
		tmp[j] = Pt[j] * Qt[j];

	std::vector<double> ret(total);
	inv_radix2fft(tmp, total, num_threads, std::ref(ret));
	return ret;
}
