#pragma once

#include <type_traits>
#include <functional>
#include "function_traits.hpp"

template<typename T>
struct ViewTraits
{
   typedef decltype(std::declval<T>().read()) ViewValue_t;
};

template<typename T>
void advance_to_first_valid(T& f)
{
   while(!f.done() && !std::invoke(f.pred, f.read()))
   {
      f.next();
   }
}

template<typename View_t>
struct FilterFun : View_t
{
   typedef typename ViewTraits<View_t>::ViewValue_t ViewValue_t;
   typedef typename std::function<bool(ViewValue_t)> Pred_t;
   FilterFun(View_t const& v, Pred_t p)
      : View_t(v)
      , pred(p)
   {
      advance_to_first_valid(*this);   
   }

   void next()
   {
      View_t::next();
      advance_to_first_valid(*this);
   }

   Pred_t pred;
};

template<typename T>
struct FilterOp
{
   std::function<bool(T)> params;
};

template<typename T>
auto filter(T const& p)
{
   // static_assert(std::is_convertible_v<T, std::function>); 
   // static_assert(std::is_function_v<T>); // doesn't works with lambdas 
   typedef function_traits<T> PredTraits;
   static_assert(PredTraits::arity == 1); 
   typedef typename PredTraits::template arg<0>::type Filter_type; 
   return FilterOp<Filter_type> { p };
}

template<typename View_t>
FilterFun<View_t> operator |(View_t const& right, FilterOp<typename ViewTraits<View_t>::ViewValue_t> const& left)
{
   return FilterFun<View_t>(right, left.params);   
}
