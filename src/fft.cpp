#include <fft.hpp>
#include <algorithm>

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
	
	std::vector<double> U(half_n, 0);
	std::vector<double> V(half_n, 0);

	for (size_t j = 0; j < half_n; j++) {
		U[j] = P[2*j];
		V[j] = P[2*j+1];
	}

	ComVector Ut(half_n, 0);
	ComVector Vt(half_n, 0);
	// Make one thread run radix2fft and do the other yourself
	if (num_threads >= 2) {
		std::thread worker(&radix2fft, std::ref(U), half_n, half_threads1, std::ref(Ut));
		radix2fft(V, half_n, half_threads2, std::ref(Vt));
		worker.join();
	}
	else {
		radix2fft(U, half_n, half_threads1, std::ref(Ut));
		radix2fft(V, half_n, half_threads2, std::ref(Vt));
	}
	
	std::complex<double> coeff(0, M_PI*2/double(n));
	std::complex<double> omega_1 = std::pow(M_E, coeff);
	std::complex<double> omega = 1.;

	for (size_t j = 0; j < half_n; j++) {
		res[j] = Ut[j] + omega*Vt[j];
		res[j+half_n] = Ut[j] - omega*Vt[j];
		omega *= omega_1;
	}
}

void inv_radix2fft(ComVector &P, size_t n, size_t num_threads, ComVector &res) {
	if (n == 1) {
		res[0] = P[0];
	}
	if (n == 0) {return;}

	size_t half_n = n/2;
	double n_d = double(n);
	size_t half_threads1 = num_threads/2;
	size_t half_threads2 = half_threads1 + (num_threads % 2);
	
	ComVector U(half_n, 0);
	ComVector V(half_n, 0);

	for (size_t j = 0; j < half_n; j++) {
		U[j] = P[2*j];
		V[j] = P[2*j+1];
	}

	ComVector Ut(half_n, 0);
	ComVector Vt(half_n, 0);
	if (num_threads >= 2) {
		std::cout << "Spawn thread" << std::endl;
		std::thread worker(&inv_radix2fft, std::ref(U), half_n, half_threads1, std::ref(Ut));
		inv_radix2fft(V, half_n, half_threads2, std::ref(Vt));
		worker.join();
	}
	else {
		inv_radix2fft(U, half_n, half_threads1, Ut);
		inv_radix2fft(V, half_n, half_threads2, Vt);
	}
	
	std::complex<double> coeff(0, (-2.)*M_PI/n_d);
	std::complex<double> omega_1 = std::pow(M_E, coeff);
	std::complex<double> omega = 1.;

	for (size_t j = 0; j < half_n; j++) {
		res[j] = (Ut[j] + omega*Vt[j])/2.;
		res[j+half_n] = (Ut[j] - omega*Vt[j])/2.;
		omega *= omega_1;
	}
}

std::vector<double> fft_poly_mult(double *p1, size_t n1, double *p2, size_t n2, size_t num_threads) {
	size_t sum = (n1+n2-1);
	size_t total = 1;
	while (total < sum) total = total << 1;
	
	std::vector<double> P(total, 0.);
	std::vector<double> Q(total, 0.);
	ComVector tmp(total);

	for (size_t i = 0; i < n1; i++) {P[i] = p1[i];}
	for (size_t i = 0; i < n2; i++) {Q[i] = p2[i];}

	size_t half_threads1 = num_threads/2;
	size_t half_threads2 = half_threads1 + (num_threads % 2);
	ComVector Pt(total, 0);
	ComVector Qt(total, 0);
	radix2fft(P, total, half_threads1, std::ref(Pt));
	radix2fft(Q, total, half_threads2, std::ref(Qt));
	
	for (size_t j = 0; j < total; j++) 
		tmp[j] = Pt[j] * Qt[j];

	ComVector ret_c(total, 0);
	inv_radix2fft(tmp, total, num_threads, std::ref(ret_c));
	std::vector<double> ret(total, 0);
	auto begin = ret_c.begin();
	for (size_t i =0; i < sum; i++) {
		ret[i] = begin->real();
		begin++;
	}
	return ret;
}

void trim_less_than(ComVector &P, double cutoff) {
    for (size_t i = 0; i < P.size(); i++) {
        if (std::norm(P[i]) < cutoff)
            P[i] = 0;
    }
}


size_t dist(size_t a, size_t b) {
    return std::sqrt(std::pow(a,2) - std::pow(b,2));
}

void trim_signal(ComVector &P, size_t low, size_t high) {
    size_t mid = P.size()/2;
    for (size_t i = 0; i < low; i++) {
        if (dist(i,mid) < low)
            P[i] = 0;
    }
    for (size_t i = high; i < P.size(); i++) {
        if (dist(i,mid) > high)
            P[i] = 0;
    }
}

//void trim_n_perc_of_max(ComVector &P, double perc) {
    //auto it = std::max_element(P.begin(), P.end(),
            //[](std::complex<double> a, std::complex<double> b){
            //return std::norm(a) < std::norm(b);
            //});
    //std::complex<double> max = *it;
    //double cutoff = perc*std::norm(max)/100.0;
//
    //for (size_t i = 0; i < P.size(); i++) {
        //if (std::norm(P[i]) < cutoff)
            //P[i] = 0;
    //}
//}
