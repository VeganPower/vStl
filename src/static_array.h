#pragma once

#include "config.h"
#include "views/array_view.hpp"
#include "elems.h"

#include "memory_buffer.h"
#include <type_traits>
#include <initializer_list>

namespace vstl
{

template<typename T, size_t alignment = std::alignment_of<T>::value>
class StaticArray
{
public:
   typedef  typename std::remove_cv<T>::type element;
   typedef element& reference;
   typedef element const& const_reference;
   typedef element* pointer;
   typedef element const* const_pointer;

   typedef uint32_t size_type; // 4 bilions of elements should be enought for everyone (Bill Gates)
   static const size_t k_elem_size = sizeof(element);
   // Initilize the array with predefined capacity
   StaticArray(size_type init_capacity)
      : memory_(init_capacity * k_elem_size, alignment)
      , size_(init_capacity)
   {
      for (int i = 0; i < size_; ++i)
      {
         new(get() + i)T();
      }
   }
#if __cpp_initializer_lists
   StaticArray(std::initializer_list<T> init)
      : memory_(init.size() * k_elem_size, alignment)
      , size_(init.size())
   {
      size_type i = 0;
      for (auto v : init)
      {
         new(get() + i)T(v);
         ++i;
      }
   }
#endif
   StaticArray(StaticArray<T, alignment> const& to_copy)
      : memory_ { to_copy.size() * k_elem_size, alignment }
      , size_ { to_copy.size_ }
   {
      ElemFacade<T, std::is_pod<T>::value >::construct_elements(get(), to_copy.get(), to_copy.size());
   }

   StaticArray(StaticArray<T, alignment>&& to_move)
      : memory_ { std::move(to_move) }
      , size_ { to_move.size_ }
   {
   }

   ~StaticArray()
   {
      ElemFacade<T, std::is_pod<T>::value >::destroy_elements(get(), size());
   }

/*
   StaticArray(ConstPtrRange<T> const& range)
      : memory_(range.distance() * k_elem_size, alignment)
      , size_(range.distance())
   {
      ElemFacade<T, std::is_pod<T>::value >::assign_elements(get(), range.begin(), size_);
   }
*/
   T& operator[](size_t t)
   {
      return get()[t];
   }

   T const& operator[](size_t t) const
   {
      return get()[t];
   }

//   auto iter() const -> ConstPtrRange<T>
//   {
//      return ConstPtrRange<T>(get(), get() + size()); // to we really want use range based iterator?
//   }

   auto iter()
   {
      return view::Array<T>(get(), get() + size());
   }

   constexpr size_t size() const
   {
      return static_cast<size_t>(size_);
   }

   size_t capacity() const
   {
      return static_cast<size_t>(size_);
   }
private:
   pointer get() { return reinterpret_cast<pointer>(memory_.ptr()); }
   const_pointer get() const { return reinterpret_cast<const_pointer>(memory_.ptr()); }

   MemoryBuffer memory_;
   size_type size_;
};

}