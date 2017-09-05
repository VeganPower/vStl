

#include <iostream>

#include <growable_vector.h>

using namespace vstl;

template<typename T>
void print_a(GrowableArray<T> const& a)
{
   for (auto const& i : a.iter())
   {
      std::cout << i << "\n";
   }
   std::cout << std::endl;
}

int main()
{
   GrowableArray<int> a = { 1, 2, 3, 4, 5 };

   // Meh
   print_a(a);

   for (int i = 0; i < 10; ++i)
      a.push_back(a.size()+1);

   print_a(a);

   a.resize(3);
   print_a(a);

   GrowableArray<std::string> b { "abc", "cde", "efg" };
   GrowableArray<std::string> c = b;
   print_a(b);
   print_a(c);

   b = c;

   print_a(b);

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