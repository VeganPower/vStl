#pragma once

namespace vstl::view
{

template<typename Cursor>
struct Traits
{};

// Specialization for pointers
template<typename T>
struct Traits<T*>
{
   constexpr static T& read(T* ptr)
   {
      return *ptr;
   }
   constexpr static void next(T* &ptr)
   {
      ++ptr;
   }
   constexpr static void prev(T* &ptr)
   {
      --ptr;
   }
   constexpr static void displace(T* &ptr, ptrdiff_t n)
   {
      ptr += n;
   }
   constexpr static ptrdiff_t distance(T* left, T* right)
   {
      return left - right;
   }
   constexpr static bool equal(T* left, T* right)
   {
      return left == right;
   }
};

}