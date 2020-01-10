#include <iostream>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <omp.h>


const int N = 5;
const int FS = 38;

int fib(int n) {
   if (n < 2) {
      return (n);
   } else {
      int x = fib(n - 1);
      int y = fib(n - 2);
	   return (x + y);
   }
}

using shared_pair = std::shared_ptr<std::pair<int, int>>;

void processwork(shared_pair &ptr)
{
   int n = ptr->first;
   ptr->second = fib(n);
}

int main(int argc, char *argv[]) {
   std::cout << "Process linked list\n"
             << "  Each linked list node will be processed by function 'processwork()'\n"
             << "  Each ll node will compute " << N
             << "fibonacci numbers beginning with " << FS
             << "\n";

   std::list<shared_pair> link;
   for (int i=0; i<N+1; ++i) {
      std::pair<int, int> pair(FS+i, i);
      auto ptr = std::make_shared<std::pair<int, int>>(pair);
      link.push_back(ptr);
   }

   double start = omp_get_wtime();

   std::vector<shared_pair> vec;
   for (auto& ptr : link)
      vec.push_back(ptr);

   #pragma omp parallel for
   for (auto& pair : vec)
      processwork(pair);

   double end = omp_get_wtime();

   for (auto& ptr : link)
      std::cout << "<" << ptr->first << ", " << ptr->second << "> ";
   std::cout << "\nCompute Time: " << end - start << "seconds\n";
}
