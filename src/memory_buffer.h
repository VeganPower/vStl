#include "v_assert.h"
#include "config.h"

#if __cpp_exceptions
#include <new>
#endif

#include <limits>
#include <memory>
#include <stdlib.h>
#include <string.h>

namespace vstl
{

// Handle buffer up to 4GiB
class MemoryBuffer
{
   typedef uint32_t size_type;
   typedef uint32_t align_type;
public:
   MemoryBuffer(size_t s, size_t a)
      : ptr_ { aligned_alloc(a, s) }
      , size_ { static_cast<size_type>((ptr_ != nullptr) ? s : 0) }
      , alignment_ { static_cast<align_type>(a) }
   {
      V_ASSERT(size() < std::numeric_limits<size_type>::max());
   #if __cpp_exceptions
      if (ptr_ == nullptr && size() > 0)
         {
            throw(std::bad_alloc());
         }
   #endif
   }

   MemoryBuffer(MemoryBuffer const & to_copy)
      : ptr_ { aligned_alloc(to_copy.alignment(), to_copy.size()) }
      , size_{ to_copy.size_ }
      , alignment_{ to_copy.alignment_ }
   {
      memcpy(ptr_, to_copy.ptr_, size_);
   }

   MemoryBuffer(MemoryBuffer && to_move)
      : ptr_ { to_move.ptr_ }
      , size_ { to_move.size_ }
      , alignment_ { to_move.alignment_ }
   {
      to_move.ptr_ = nullptr;
      to_move.size_ = 0;
      to_move.alignment_ = 0;
   }

   auto operator = (MemoryBuffer const & to_copy) -> MemoryBuffer&
   {
      void* ptr = nullptr;
      if (size_ < to_copy.size())
      {
         ptr = realloc(ptr_, to_copy.size());
         V_ASSERT(ptr == ptr_);
      }
      else
      {
         ptr = aligned_alloc(to_copy.alignment(), to_copy.size());
      }
      #if __cpp_exceptions
      if (ptr == nullptr && to_copy.size() > 0)
      {
         throw(std::bad_alloc());
      }
      #endif
      memcpy(ptr, to_copy.ptr_, to_copy.size());
      ptr_ = ptr;
      free(ptr);

      size_ = to_copy.size_;
      alignment_ = to_copy.alignment_;
      return *this;
   }

   auto operator = (MemoryBuffer&& to_move) -> MemoryBuffer&
   {
      return *this;
   }

   ~MemoryBuffer()
   {
      free(ptr_);
   }

   operator void* ()
   {
      return ptr_;
   }

   operator const void* () const
   {
      return ptr_;
   }

   void* ptr() { return ptr_; }
   void const* ptr() const { return ptr_; }
   size_t size() const { return static_cast<size_t>(size_); }
   size_t alignment() const { return static_cast<size_t>(alignment_); }
private:
   void* ptr_ = nullptr;
   size_type size_ = 0;
   align_type alignment_ = 0;
};

inline void copy(MemoryBuffer const& src, MemoryBuffer& dst)
{
   V_ASSERT(src.size() <= dst.size());
   memcpy(dst, src, src.size());
}

}

namespace std
{

inline void swap(vstl::MemoryBuffer& a, vstl::MemoryBuffer& b)
{
   vstl::MemoryBuffer t = b;
   b = std::move(a);
   a = std::move(t);
}

}