#pragma once
#include "config.h"
#include <cstring>
#include <type_traits>

// avoid calling constructor/destructor for POD
// This is probably the dumbest idea ever as the compiler alread do this job for me.
namespace vstl
{

template<typename T, bool is_pod>
struct ElemFacade;

template<typename T>
struct ElemFacade<T, true>
{
   static void construct_elements(T* dest, T const* src, size_t count)
   {
      memcpy(dest, src, count * sizeof(T));
   }

   static void assign_elements(T* dest, T const* src, size_t count)
   {
      memcpy(dest, src, count * sizeof(T));
   }
   static void move_elements(T* dest, T* src, size_t count)
   {
      // nothing to do
   }
   static void destroy_elements(T* elems, size_t count)
   {
      // nothing to do
   }
};

template<typename T>
struct ElemFacade<T, false>
{
   static void construct_elements(T* dest, T const* src, size_t count);
   static void assign_elements(T* dest, T const* src, size_t count);
   static void move_elements(T* dest, T* src, size_t count);
   static void destroy_elements(T* elems, size_t count);
};

template<typename T>
void ElemFacade<T, false>::construct_elements(T* dest, T const* src, size_t count)
{
   for (size_t i = 0; i < count; ++i)
   {
      new(dest+i)T(src[i]);
   }
}

template<typename T>
void ElemFacade<T, false>::assign_elements(T* dest, T const* src, size_t count)
{
   for (size_t i = 0; i < count; ++i)
   {
      dest[i] = src[i];
   }
}

template<typename T>
void ElemFacade<T, false>::move_elements(T* dest, T* src, size_t count)
{
   for (size_t i = 0; i < count; ++i)
   {
      dest[i] = std::move(src[i]);
   }
}

template<typename T>
void ElemFacade<T, false>::destroy_elements(T* elems, size_t count)
{
   for (size_t i = 0; i < count; ++i)
   {
      (elems+i)->~T();
   }
}

}