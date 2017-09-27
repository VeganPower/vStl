#pragma once

#include "config.h"
#include <type_traits>

namespace vstl
{

template<typename T, typename Iterator, typename IteratorEnd = Iterator>
class Range
{
public:
   typedef typename std::remove_const<T>::type type;
   typedef type* ptr;
   typedef const type* const_ptr;
   typedef type& reference;
   typedef type const& const_reference;

   typedef Iterator iterator;
   typedef IteratorEnd iterator_end;
   Range(iterator b, iterator_end e)
      : b_(b)
      , e_(e)
   {}
   iterator begin() const { return b_; }
   iterator_end end() const { return e_; }

private:
   const iterator b_;
   const iterator_end e_;
};

template<typename T>
struct PtrRange : public Range<T, typename std::remove_cv<T>::type*>
{
   typedef typename std::remove_const<T>::type type;
   typedef type* ptr;
   typedef const type* const_ptr;
   PtrRange(ptr b, ptr e)
      : Range<T, T*>(b, e)
   {}

   void advance(ptr p)
   {
      ++p;
   }

   size_t distance() const
   {
      return Range<T, T*>::end() - Range<T, T*>::begin();
   }
};

template<typename T>
struct ConstPtrRange : public Range<T, typename std::remove_cv<T>::type const*>
{
   typedef typename std::remove_cv<T>::type type;
   typedef type const* ptr;
   typedef type const* const_ptr;

   ConstPtrRange(ptr b, ptr e)
    : Range<T, T*>(b, e)
    {}

   ConstPtrRange(PtrRange<T> const& r)
      : Range<T, ptr>(r.begin(), r.end())
   {}

   void advance(ptr p)
   {
      ++p;
   }

   size_t distance() const
   {
      return Range<T, ptr>::end() - Range<T, ptr>::begin();
   }
};

template<typename T>
void advance(T& it)
{
   it++;
}

}