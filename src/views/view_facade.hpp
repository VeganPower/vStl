#pragma once

#include "iterator.hpp"

namespace vstl::view
{

// Facade add begin and end to a view
template<typename View>
struct Facade
{
   constexpr auto begin()
   {
      int* c = get().cursor();
      return Iterator<int* >(c);
   }
   constexpr auto end()
   {
      int*  s = get().sentinel();
      return Iterator<int* >(s);
   }
   constexpr auto begin() const
   {
      int const*  c = get().cursor();
      return Iterator<int const*>(c);
   }
   constexpr auto end() const
   {
      int const* s = get().sentinel();
      return Iterator<int const*>(s);
   }
private:
   constexpr View& get()
   {
      return *static_cast<View*>(this);
   }
   constexpr View const& get() const
   {
      return *static_cast<View const*>(this);
   }
};

}