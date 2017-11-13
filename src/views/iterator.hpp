#pragma once
#include "view_traits.hpp"
#include <type_traits>

namespace vstl
{

template<typename Cursor>
struct Iterator
{
   Iterator(int const* c)
      : cursor_(c)
   {}
   // pointer
   // const pointer
   typedef typename std::remove_pointer<Cursor>::type element;
   typedef typename std::add_lvalue_reference<element>::type reference;
   typedef typename std::add_const<reference>::type const_reference;

   // only for non const interator
   reference  operator*()
   {
      return view::Traits<Cursor>::read(cursor_);
   }

   // const_reference operator*() const
   // {
   //    return view_traits<Cursor>::read(c);
   // }

   Iterator operator++()
   {
      // ++cursor_;
      view::Traits<Cursor>::next(cursor_);
      return *this;
   }

   friend bool operator==(Iterator const& left, Iterator const& right)
   {
      return view::Traits<Cursor>::equal(left.cursor_, right.cursor_);
   }
   friend bool operator!=(Iterator const& left, Iterator const& right)
   {
      return !(left == right); 
   }

   Cursor cursor_;
};

}
