#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "omp.h"


double rect_area(const int idx, const double step) {
    double x = (idx + 0.5) * step;
    return 4.0 / (1.0 + x*x);
}


double calc_pi(const double num_steps, const int num_threads) {
    double step = 1.0 / static_cast<double>(num_steps);
    std::vector<double> thread_sums(num_threads);

    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        for (int i = id; i < num_steps; i += num_threads) {
            thread_sums[id] += rect_area(i, step);
        }
    }

    double sum = std::accumulate(thread_sums.begin(), thread_sums.end(), 0.0);
    return step * sum;
}


int main(int argc, char **argv) {
    long num_steps;
    if (argc > 1) {
        num_steps = std::stol(argv[1]);
    } else {
        num_steps = 100'000;
    }

    int num_threads;
    if (argc > 2) {
        num_threads = std::stoi(argv[2]);
    } else {
        num_threads = 4;
    }

    double t0 = omp_get_wtime();
    double pi = calc_pi(num_steps, num_threads);
    double t1 = omp_get_wtime();

    std::cout << "Threads: " << num_threads << '\n'
              << "Steps: "   << num_steps   << '\n'
              << "Pi: "      << pi          << '\n'
              << "Time: "    << t1 - t0     << '\n'
              << std::endl;
}
