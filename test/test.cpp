
#include <iostream>

//#include <growable_vector.h>
#include <static_array.h>
// #include <filter.h>
#include <for_each.h>
#include <views/view_traits.hpp>

#include<utility>

using namespace vstl;

template<typename View, typename F>
struct FilteredView
{
   typedef int* ViewCursor;
   typedef int* ViewSentinel;
   struct Cursor
   {
      // typedef decltype(View()).cursor(), // get traits from View 
      ViewCursor cursor_;
      F& f_;
   };
   struct ConstCursor
   {
      // typedef decltype(View()).cursor(), // get traits from View 
      ViewCursor cursor_;
      F& f_;
   };
   struct Sentinel
   {
      ViewSentinel sentinel_;
   };

   Iterator<Cursor> begin() const
   {
      auto it = view_.begin();
      auto end = view_.end();
      while(it != end)
      {
         if(f_(it))
         {
            return Iterator<Cursor>(Cursor(it, f_));
         }
         ++it;
      }
      return Iterator<Cursor>(Cursor(view_.end(), f_));
   }

   Iterator<Cursor> begin()
   {
      auto it = view_.begin();
      auto end = view_.end();
      while(it != end)
      {
         if(f_(it))
         {
            return Iterator<Cursor>(Cursor(it, f_));
         }
         ++it;
      }
      return Iterator<Cursor>(Cursor(view_.end(), f_));
   }
   auto end()
   {
      return Sentinel(view_.end());
   }
   auto end() const
   {
      return Sentinel(view_.end());
   }
   FilteredView(View &&view, F f)
      : view_(view)
      , f_(f)
   {}
   View view_;
   F f_;
};

namespace vstl::view
{
   // Specialization for pointers
template<typename View, typename F>
struct Traits<FilteredView<View, F> >
{
   typedef typename FilteredView<View, F>::Cursor cursor_type;
   typedef typename FilteredView<View, F>::ConstCursor const_cursor_type;
   // typedef int T;
   static int read(const_cursor_type& c)
   {
      return Traits<View>::read(c.cursor_);
   }
   static int& read(cursor_type& c)
   {
      return Traits<View>::read(c.cursor_);
   }

   static void next(cursor_type& c)
   {
      auto end = c.owner->end();
      while(c.cursor_ != end)
      {
         ++c.cursor_;
         if(f_(c.cursor_))
         {
            break;
         }
      }
   }
   static void next(const_cursor_type& c)
   {
      auto end = c.owner->end();
      while(c.cursor_ != end)
      {
         ++c.cursor_;
         if(f_(c.cursor_))
         {
            break;
         }
      }
   }

   static bool equal(cursor_type const& left, typename FilteredView<View, F>::Sentinel const& right)
   {
      return left.cursor_ == right.sentinel_;
   }
};

}

template<typename View, typename F> 
auto filter(View &&v, F f)
{
   return FilteredView<View, F>(std::forward<View>(v), f);
}

template<typename T>
void print_a(T const& a)
{
   for_each(a, [](int const &v)
      //typename T::const_reference it)
   {
      std::cout << v << "\n";
   });
   std::cout << std::endl;
}

int main()
{
   StaticArray<int> a = { 1, 2, 3, 4, 5 };

   print_a(a.iter());

   // for (int i = 0; i < 10; ++i)
      // a.push_back(a.size()+1);

   // print_a(a.iter());
   // print_a(a.iter()
      // | filter([](int i){ return i > 3; }));
   print_a (filter(a.iter(),
      [](int i) -> bool { return i > 3; }
   ) );

   // a.resize(3);
   // print_a(a.iter());
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