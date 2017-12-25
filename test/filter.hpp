#pragma once

#include <functional>


template<typename View_t>
struct FilterFun : View_t
{
   FilterFun(View_t const& v, std::function<bool(int)> p);

   void next();

   std::function<bool(int)> pred;
};

template<typename View_t>
void advance_to_first_valid(FilterFun<View_t>& f)
{
   while(!f.done() && !f.pred(f.read()))
   {
      f.next();
   }
}

template<typename View_t>
FilterFun<View_t>::FilterFun(View_t const& v, std::function<bool(int)> p)
   : View_t(v)
   , pred(p)
{
   advance_to_first_valid(*this);   
}

template<typename View_t>
void FilterFun<View_t>::next()
{
   View_t::next();
   advance_to_first_valid(*this);
}

struct FilterOp
{
   std::function<bool(int)> pred;
};

template<typename View_t>
FilterFun<View_t> operator |(View_t const& right, FilterOp const& left)
{
   return FilterFun<View_t>(right, left.pred);   
}

FilterOp filter(std::function<bool(int)> p)
{
   return FilterOp { p };
}
