// std::complex<double>
// -O0 3.75 seconds      -O3 0.49 seconds
#include <complex>
#include <iostream>
#include <omp.h>


constexpr size_t NPOINTS = 1000;
constexpr size_t MAXITER = 1000;

void testpoint(std::complex<double>&);

std::complex<double> c;
int numoutside = 0;

int main() {
    double eps = 1e-5;
//   Loop over grid of points in the complex plane which contains the Mandelbrot set,
//   testing each point to see whether it is inside or outside the set.

    double t0 = omp_get_wtime();

    #pragma omp parallel for private(c)
    for (size_t i = 0; i < NPOINTS; ++i) {
        for (size_t j = 0; j < NPOINTS; ++j) {
            c.real(-2.0 + 2.5 * i / NPOINTS + eps);
            c.imag(1.125 * j / NPOINTS + eps);
            testpoint(c);
        }
    }

    double t1 = omp_get_wtime();

    double area = 5.625 * (NPOINTS * NPOINTS - numoutside) / (NPOINTS * NPOINTS);
    double error = area / NPOINTS;

    std::cout << "Time " << t1 - t0 << '\n'
              << "Area of Mandlebrot set = " << area << " +/- " << error << '\n'
              << "Correct answer should be around 1.510659\n"
              << std::endl;

}

void testpoint(std::complex<double> &c) {

// Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside set
// If loop count reaches MAXITER, point is considered to be inside the set

    std::complex<double> z = c;
    double temp;

    for (size_t i = 0; i < MAXITER; ++i) {
        temp = (z.real()*z.real()) - (z.imag()*z.imag()) + c.real();
        z.imag(z.real()*z.imag()*2 + c.imag());
        z.real(temp);
        if (std::norm(z) > 4.0) {
            #pragma omp atomic
            numoutside++;
            break;
        }
    }

}
