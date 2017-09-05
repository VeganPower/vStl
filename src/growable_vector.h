
#include "config.h"
#include "range.h"

#include "memory_buffer.h"
#include <type_traits>
#include <initializer_list>

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

// The Curiously Recurring Template Pattern (CRTP)
template<class T>
class Base
{
    // methods within Base can use template to access members of Derived
};
class Derived : public Base<Derived>
{
   int a;
};

template<typename T, size_t alignment = std::alignment_of<T>::value>
class GrowableArray
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
   GrowableArray(size_type init_capacity)
      : memory_(init_capacity * k_elem_size, alignment)
      , size_(0)
   {
   }
#if __cpp_initializer_lists
   GrowableArray(std::initializer_list<T> init)
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
   GrowableArray(GrowableArray<T, alignment> const& to_copy)
      : memory_ { to_copy.size() * k_elem_size, alignment }
      , size_ { to_copy.size_ }
   {
      ElemFacade<T, std::is_pod<T>::value >::construct_elements(get(), to_copy.get(), to_copy.size());
   }

   GrowableArray(GrowableArray<T, alignment>&& to_move)
      : memory_ { std::move(to_move) }
      , size_ { to_move.size_ }
   {
   }

   GrowableArray const & operator = (GrowableArray<T, alignment> const& to_copy)
   {
      memory_.resize(to_copy.size() * k_elem_size);
      ElemFacade<T, std::is_pod<T>::value >::assign_elements(get(), to_copy.get(), to_copy.size());
      return *this;
   }

   GrowableArray const & operator = (GrowableArray<T, alignment>&& to_move)
   {
      memory_ = std::move(to_move);
      return *this;
   }


   // Move is always trivial. The memory buffer take care of it
   // GrowableArray(GrowableArray&& to_move) = default;
   // GrowableArray& operator = (GrowableArray&& to_move) = default;

   ~GrowableArray()
   {
      ElemFacade<T, std::is_pod<T>::value >::destroy_elements(get(), size());
   }
   // TODO: Rule of 5 for non POD objects

   //TODO:
   // insert
   // remove


   void push_back(T const& value)
   {
      if (capacity() == size_)
      {
         grow();
      }
      get()[size_++] = value;
   }

   T pop_back()
   {
      V_ASSERT(size_ > 0);
      return get()[--size_];
   }

   T& operator[](size_t t)
   {
      return get()[t];
   }

   T const& operator[](size_t t) const
   {
      return get()[t];
   }

   auto iter() const -> ConstRange<T>
   {
      return ConstRange<T>(get(), get() + size()); // to we really want use range based iterator?
   }

   auto iter() -> Range<T>
   {
      return Range<T>(get(), get() + size()); // to we really want use range based iterator?
   }

   void resize(size_t new_size)
   {
      memory_.resize(new_size * k_elem_size);
      size_ = new_size;
   }

   size_t size() const
   {
      return static_cast<size_t>(size_);
   }

   size_t capacity() const
   {
      return memory_.size() / k_elem_size;
   }
private:
   pointer get() { return reinterpret_cast<pointer>(memory_.ptr()); }
   const_pointer get() const { return reinterpret_cast<const_pointer>(memory_.ptr()); }

   void grow()
   {
      size_type current_size = size_;
      // should I use the golden ratio?
      size_type new_size = (current_size == 0 ? 1 : current_size * 2);
      memory_.resize(new_size * k_elem_size);
   }

   MemoryBuffer memory_;
   size_type size_;
};

/*
template < typename T, size_t alignment, typename  = typename std::enable_if < !std::is_trivially_destructible<T>::value >>
GrowableArray<T, alignment>::~GrowableArray()
{
   for (size_type i = 0; i < size_; ++i)
   {
      get[i].~T();
   }
}
*/
}

