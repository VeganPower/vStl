#include "static_array.h"
#include <iostream>
#include <functional>

#include "filter.hpp"

// cartesian product
// for (int i = 0; i != end; ++i)

struct IntView
{
   int* cursor = nullptr;
   int* sentinel = nullptr;
   
   void next()
   {
      cursor++;
   }

   int read()
   {
      return *cursor;
   }

   bool done()
   {
      return cursor == sentinel;
   }
};

template<typename View_t, typename result_type, typename filter_type>
struct TransformFun : public View_t
{
   TransformFun(View_t const&v, std::function<result_type(filter_type)> p)
      : View_t(v)
      , pred(p)
   {}

   result_type read()
   {
      return pred(View_t::read());
   }

   std::function<result_type(filter_type)> pred;
};

template<typename T>
struct TransformOp
{
   T pred;
   // std::function<int(int)> pred;
};

template<typename View_t, typename T>
auto operator |(View_t const& right, TransformOp<T> const& left)
{
   typedef function_traits<T> PredTraits;
   static_assert(PredTraits::arity == 1); 
   typedef typename PredTraits::template arg<0>::type filter_type; 
   typedef typename PredTraits::result_type result_type; 

   return TransformFun<View_t, result_type, filter_type>(right, left.pred);   
}

template<typename T>
TransformOp<T> transform(T const& p)
{
   return TransformOp<T>{ p };
}

template<typename View_t>
struct TakeFun : public View_t
{
   TakeFun(View_t const& v, int c)
      : View_t(v)
      , count(c)
   {}
   int count = 0;
   
   void next()
   {
      count--;
      View_t::next();
   }

   bool done()
   {
      return (count == 0) || View_t::done();
   }
};

struct TakeOp
{
   int param;
};

template<typename View_t>
TakeFun<View_t> operator |(View_t const& right, TakeOp const& left)
{
   return TakeFun<View_t>(right, left.param);   
}

TakeOp take(int p)
{
   return TakeOp{ p };
}

int main()
{
   vstl::StaticArray<int> a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

   // a.iter() | filter(x>3) | transform(x*x) | take(4)
   IntView view = { &a[0], &a[a.size()] };
   for (auto t = view
               | filter( [](int x)  { return x > 3; })
               | transform( [](int x) -> float { return x*x+0.5f; })
               | take(40);
         !t.done(); t.next()
      )
   {
      auto const& x = t.read();
      std::cout << x << " ";
   }
   std::cout << std::endl;
   return 0;
}