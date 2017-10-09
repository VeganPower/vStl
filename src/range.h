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
   /*
   Range(iterator b, iterator_end e)
      : b_(b)
      , e_(e)
   {}
   iterator begin() const { return b_; }
   iterator_end end() const { return e_; }

private:
   const iterator b_;
   const iterator_end e_;*/
};

template<typename T>
struct PtrRange : public Range<T, typename std::remove_cv<T>::type*>
{
   typedef Range<T, typename std::remove_cv<T>::type*> Base;
   typedef typename std::remove_const<T>::type type;
   typedef type* ptr;
   typedef const type* const_ptr;
   
   // typedef ptr iterator;
   // typedef ptr iterator_end;

   PtrRange(ptr b, ptr e)
    : b_{b}
    , e_{e}
   {}

   bool compare(typename Base::iterator const& a, typename Base::iterator_end const& b)
   {
      return a != b;
   }

   type& get(typename Base::iterator& it)
   {
      return *it;
   }

   bool advance(ptr& p)
   {
      if (compare(p, end()))
      {
         return false;
      }
      ++p;
      return true;
   }

   size_t distance() const
   {
      return Range<T, T*>::end() - Range<T, T*>::begin();
   }

   typename Base::iterator begin() const { return b_; }
   typename Base::iterator_end end() const { return e_; }

private:
   const typename Base::iterator b_;
   const typename Base::iterator_end e_;
};

template<typename T>
struct ConstPtrRange // : public Range<T, typename std::remove_cv<T>::type const*>
{
   typedef typename std::remove_cv<T>::type type;
   typedef type const* ptr;
   typedef type const* const_ptr;
   typedef ptr iterator;
   typedef ptr iterator_end;

   ConstPtrRange(ptr b, ptr e)
    : b_{b}
    , e_{e}
    {}

   ConstPtrRange(PtrRange<T> const& r)
      : ConstPtrRange<T>(r.begin(), r.end())
   {}

   iterator begin() const { return b_; }
   iterator_end end() const { return e_; }

   static type const& get(iterator& it)
   {
      return *it;
   }

   static void advance(ptr p)
   {
      ++p;
   }

   static size_t distance()
   {
      return 4;//Range<T, ptr>::end() - Range<T, ptr>::begin();
   }
   

private:
   const iterator b_;
   const iterator_end e_;
};

template<typename T>
void advance(T& it)
{
   it++;
}

}