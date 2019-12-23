#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>
#include "omp.h"


#define PAD 8

double rect_area(const int idx, const double step) {
    double x = (idx + 0.5) * step;
    return 4.0 / (1.0 + x*x);
}


double calc_pi(const double num_steps, size_t &num_threads) {
    double step = 1.0 / static_cast<double>(num_steps);
    std::vector<double[PAD]> thread_sums(num_threads);

    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        if (!id) {
            num_threads = omp_get_num_threads();
        }
        for (int i = id; i < num_steps; i += num_threads) {
            thread_sums[id][0] += rect_area(i, step);
        }
    }

    double sum = std::accumulate(thread_sums.begin(), thread_sums.end(), 0.0,
                                 [](const auto acc, const auto it){ return acc + it[0]; });
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
    long num_steps = arg_to_var<long>(100'000, argc, argv);

    double t0 = omp_get_wtime();
    double pi = calc_pi(num_steps, num_threads);
    double t1 = omp_get_wtime();

    std::cout << "Threads: " << num_threads << '\n'
              << "Steps: "   << num_steps   << '\n'
              << "Pi: "      << pi          << '\n'
              << "Time: "    << t1 - t0     << '\n'
              << std::endl;
}
