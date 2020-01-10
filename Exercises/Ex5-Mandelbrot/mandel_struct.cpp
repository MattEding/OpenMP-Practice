// struct complex_d
// -O0 0.88 seconds      -O3 0.48 seconds
#include <iostream>
#include <omp.h>

# define NPOINTS 1000
# define MAXITER 1000

struct d_complex{
   double r;
   double i;
};

void testpoint(struct d_complex);

struct d_complex c;
int numoutside = 0;


int main() {
    double eps = 1e-5;
//   Loop over grid of points in the complex plane which contains the Mandelbrot set,
//   testing each point to see whether it is inside or outside the set.

    double t0 = omp_get_wtime();

    #pragma omp parallel for private(c)
    for (int i = 0; i < NPOINTS; ++i) {
        for (int j = 0; j < NPOINTS; ++j) {
            c.r = -2.0 + 2.5 * i / NPOINTS + eps;
            c.i = 1.125 * j / NPOINTS + eps;
            testpoint(c);
        }
    }

    double t1 = omp_get_wtime();

    double area = 5.625 * (NPOINTS * NPOINTS - numoutside) / (NPOINTS * NPOINTS);
    double error = area / (NPOINTS);

    std::cout << "Time " << t1 - t0 << '\n'
              << "Area of Mandlebrot set = " << area << " +/- " << error << '\n'
              << "Correct answer should be around 1.510659\n"
              << std::endl;

}

void testpoint(struct d_complex c){

// Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside set
// If loop count reaches MAXITER, point is considered to be inside the set

    struct d_complex z  = c;
    double temp;

    for (int i = 0; i < MAXITER; ++i) {
        temp = (z.r*z.r) - (z.i*z.i) + c.r;
        z.i = z.r*z.i*2 + c.i;
        z.r = temp;
        if ((z.r*z.r + z.i*z.i) > 4.0) {
            #pragma omp atomic
            numoutside++;
            break;
        }
    }

}
