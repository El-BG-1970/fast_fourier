#include <iostream>
#include "threadpool.hpp"
#include "fft.hpp"
#include "csv_utils.hpp"
#include <chrono>
#include <random>

void increment(int *i) { *i += 1; }

int main(int argc, char* argv[]) {
	if (argc == 1){return 0;}
	int flag = std::stoi(argv[1]);
	size_t i;
	switch (flag) {
		case 1:{
			if (argc != 4) {return 1;}
			size_t polysize = std::stoi(argv[2]);
			size_t num_threads = std::stoi(argv[3]);

			std::random_device rnd_device;
			std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
    		std::uniform_int_distribution<int> dist {-50, 50};
			auto gen = [&dist, &mersenne_engine](){
                return dist(mersenne_engine);
            };
			std::vector<double> p1(polysize);
			std::vector<double> p2(polysize);

			std::generate(p1.begin(), p1.end(), gen);
			std::generate(p2.begin(), p2.end(), gen);

			int64_t total = 0;
			for (int i = 0; i < 10; i++) {
				auto start = std::chrono::steady_clock::now();
				std::vector<double> p3 = fft_poly_mult(std::ref(p1), std::ref(p2), num_threads);
				auto finish = std::chrono::steady_clock::now();
				auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
				total += elapsed;
			}
			std::cout << total/10 << std::endl;
		break;}

		case 2:{	
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
		break;}

		case 3:{
			if (argc != 3){return 1;};
			double percentage = std::stod(argv[2]);
			std::vector<double> *data = read_data("./data/date_v_temp.csv");
			size_t ds = data->size();
			size_t sz = 1;
			while (sz < ds) sz = sz << 1;

			for (size_t i = ds; i < sz; i++) data->push_back(0.);
			std::vector<std::complex<double>> samples(sz);
			radix2fft(std::ref(*data), sz, 2, std::ref(samples));

			// here we let go of some coefficients (second parameter is percent)
			trim_less_than(std::ref(samples), percentage);
			
			// here we need to do the reverse fft in order to obtain the compressed data
			std::vector<std::complex<double>> ret_c(sz, 0.);
			inv_radix2fft(std::ref(samples), sz, 2, std::ref(ret_c));
			std::vector<double> ret_c_double(sz, 0.);
			for (size_t i = 0; i < ds; i++)
				ret_c_double[i] = std::real(ret_c[i]);
			
			// finally, we need to write a csv with the original data and the compressed data, for comparison
			write_data("./data/date_v_temp_FFT.csv", std::ref(ret_c_double), ds);

			double perf = 0;
			for (size_t i = 0; i < ds; i++) perf += std::abs((*data)[i] - ret_c_double[i]);
			perf /= ds;
			std::cout << "absolute error: " << perf << std::endl;

			double rel_perf = 0;
			for (size_t i = 0; i < ds; i++) rel_perf += std::abs(((*data)[i]-ret_c_double[i])/(*data)[i]);
			rel_perf /= ds;
			std::cout << "relative error: " << 100.0*rel_perf << "%" << std::endl;
		break;}
	}

	return 0;
}
