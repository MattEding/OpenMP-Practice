#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>
#include "omp.h"


double calc_pi(const size_t num_steps, size_t &num_threads) {
    double step = 1.0 / static_cast<double>(num_steps);
    double sum = 0.0;

    omp_set_num_threads(num_threads);
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        if (!id) {
            num_threads = omp_get_num_threads();
        }

        double partial_sum = 0.0;
        for (int i = id; i < num_steps; i += num_threads) {
            double x = (i + 0.5) * step;
            partial_sum += 4.0 / (1.0 + x*x);
        }

#pragma omp critical
        sum += partial_sum;
    }

    return step * sum;
}


size_t arg_idx = 0;
template<typename T>
T arg_to_var(const T default_val, const int argc, const char * const *argv) {
    T var;
    if (argc > ++arg_idx) {
        std::istringstream iss(argv[arg_idx]);
        iss >> var;
    } else {
        var = default_val;
    }
    return var;
}


int main(int argc, char **argv) {
    size_t num_threads = arg_to_var<size_t>(4, argc, argv);
    size_t num_steps = arg_to_var<size_t>(100'000, argc, argv);

    double t0 = omp_get_wtime();
    double pi = calc_pi(num_steps, num_threads);
    double t1 = omp_get_wtime();

    std::cout << "Threads: " << num_threads << '\n'
              << "Steps: "   << num_steps   << '\n'
              << "Pi: "      << pi          << '\n'
              << "Time: "    << t1 - t0     << '\n'
              << std::endl;
}
