
#include <iostream>

#include <growable_vector.h>
#include <static_array.h>
#include <filter.h>
#include <for_each.h>

using namespace vstl;

template<typename T>
void print_a(T const& a)
{
   // meh
   for_each(a, [](int it)
      //typename T::const_reference it)
   {
      std::cout << it << "\n";
   });
   std::cout << std::endl;
}

int main()
{
   GrowableArray<int> a = { 1, 2, 3, 4, 5 };

   print_a(a.iter());

   for (int i = 0; i < 10; ++i)
      a.push_back(a.size()+1);

   print_a(a.iter());
   // print_a<int>(a.iter()
      // | filter([](int i){ return i > 3; }));
  print_a(filter(a.iter(),
      [](int i) -> bool { return i > 3; })
  );

   a.resize(3);
   print_a(a.iter());
#if 0
   GrowableArray<std::string> b { "abc", "cde", "efg" };
   StaticArray<std::string> c { b.iter() };
   print_a(b.iter());
   print_a(c.iter());
   GrowableArray<std::string> d;
   d = b;

   print_a(d.iter());
#endif

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