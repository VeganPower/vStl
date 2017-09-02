
#include "config.h"
#include "range.h"

#include "memory_buffer.h"
#include <type_traits>
#include <initializer_list>


namespace vstl
{

template<typename T, size_t alignment = std::alignment_of<T>::value>
class GrowableVector
{
public:
   typedef T Type;
   typedef Type* Ptr;
   static const size_t k_elem_size = sizeof(T); 
   GrowableVector(size_t init_size)
      : memory_(init_size * k_elem_size, alignment)
      , size_(init_size)
   {
   }
#if __cpp_initializer_lists
   GrowableVector(std::initializer_list<T> init)
      : memory_(init.size() * k_elem_size, alignment)
      , size_(init.size())
   {
      size_t i = 0;
      for(auto v : init)
      {
         new(ptr() + i)T(v);
         ++i;
      }
   }
#endif
   // TODO: Rule of 5 for non POD objects

   //TODO:
   // insert
   // remove
   // 

   auto iter() const -> ConstRange<T>
   {
      return ConstRange<T>(ptr(), ptr()+size()); // to we really want use range based iterator?
   }
   auto iter() -> Range<T> 
   {
      return Range<T>(ptr(), ptr() + size()); // to we really want use range based iterator?
   }
   size_t size()
   {
      return size_;
   }
   size_t capacity()
   {
      return memory_.size() / k_elem_size;
   }
private:
   Ptr ptr() { return reinterpret_cast<Type*>(memory_.ptr()); }

   MemoryBuffer memory_;
   size_t size_;
};

}

