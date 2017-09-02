// memoryblock
// allocator
// DEFINE COMPATIBILITY WITH std::STL
// view and range
   // for each
   // find
   // any_of
   // all
   // filter
   // lazy transform
   // flattern
   // take
   // drop
// zip
// map

#include <iostream>

#include <growable_vector.h>

using namespace vstl;

int main()
{
   GrowableVector<int> a = { 1, 2, 3, 4, 5 };

   // Meh
   for (auto& i : a.iter())
   {
      std::cout << i;
   }

   /*
    TODO:
   Fixed vector allocated on the heap
   struct A
   {
      int x;
      float y;
   }
   FixedVector<A> a = {
            A(0, 1.0),
            A(0, 9.0),
            A(0, 6.0),
            A(1, 2.0),
            A(1, 4.0),
         };
   auto it = find(transform(a, std::mem_fn(&A::x)), 1);
   std::cout << *it << "\n"; // 2.0
   it.base() == 
   */
}