#include <iostream>
#include "omp.h"

int main() {
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        std::cout << "Hello(" << ID << ") world(" << ID << ")" << std::endl;
    }
}
