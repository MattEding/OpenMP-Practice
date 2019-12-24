#include <iostream>
#include <sstream>
#include "omp.h"


double calc_pi(const size_t num_steps) {
    double step = 1.0 / static_cast<double>(num_steps);
    double sum = 0.0;

#pragma omp parallel for reduction(+: sum)
    for (int i = 0; i < num_steps; ++i) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x*x);
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
    size_t num_steps = arg_to_var<size_t>(100'000, argc, argv);

    double t0 = omp_get_wtime();
    double pi = calc_pi(num_steps);
    double t1 = omp_get_wtime();

    std::cout << "Steps: "   << num_steps   << '\n'
              << "Pi: "      << pi          << '\n'
              << "Time: "    << t1 - t0     << '\n'
              << std::endl;
}
