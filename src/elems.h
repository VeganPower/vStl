#pragma once
#include "config.h"

#include <utility>
#include <cstring>
#include <type_traits>

// avoid calling constructor/destructor for POD
// This is probably the dumbest idea ever as the compiler alread do this job for me.
namespace vstl
{

template<typename T>
struct ElemFacade<T>
{
   static void default_construct_elements(T* dest, T const* src, size_t count);
   static void construct_elements(T* dest, T const* src, size_t count);
   static void assign_elements(T* dest, T const* src, size_t count);
   static void move_elements(T* dest, T* src, size_t count);
   static void destroy_elements(T* elems, size_t count);
};

template<typename T>
void ElemFacade<T>::default_construct_elements(T* dest, T const* src, size_t count)
{

}

template<typename T>
void ElemFacade<T>::construct_elements(T* dest, T const* src, size_t count)
{
   if constexpr (std::is_trivially_copyable<T>::value )
   {
      memcpy(dest, src, count * sizeof(T));
   }
   else
   {
      for (size_t i = 0; i < count; ++i)
      {
         new(dest + i)T(src[i]);
      }
   }
}

template<typename T>
void ElemFacade<T>::assign_elements(T* dest, T const* src, size_t count)
{
   if constexpr (std::is_trivially_copyable<T>::value )
   {
      memcpy(dest, src, count * sizeof(T));
   }
   else
   {
      for (size_t i = 0; i < count; ++i)
      {
         dest[i] = src[i];
      }
   }
}

template<typename T>
void ElemFacade<T>::move_elements(T* dest, T* src, size_t count)
{
   if constexpr (!std::is_pod<T>::value )
   {
      for (size_t i = 0; i < count; ++i)
      {
         dest[i] = std::move(src[i]);
      }
   }
}

template<typename T>
void ElemFacade<T, false>::destroy_elements(T* elems, size_t count)
{
   if constexpr (!std::is_pod<T>::value )
   {
      for (size_t i = 0; i < count; ++i)
      {
         (elems + i)->~T();
      }
   }
}

}