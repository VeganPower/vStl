#pragma once
#include "view_facade.hpp"

namespace vstl::view
{

template<typename T>
struct Array : Facade<Array<T>>
{
   typedef T* Cursor;
   typedef T const* ConstCursor;

   Array(Cursor s, Cursor e)
      : start_{s}
      , end_{e}
   {
   }

   constexpr Cursor cursor() { return start_; }
   constexpr Cursor sentinel() { return end_; }

   constexpr ConstCursor cursor() const { return start_; }
   constexpr ConstCursor sentinel() const { return end_; }
   T* start_;
   T* end_;
};

}
