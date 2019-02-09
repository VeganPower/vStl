#pragma once

#include "v_assert.h"
#include "config.h"

#if __cpp_exceptions
#include <new>
#endif

#include <intrin.h>
#include <limits>
#include <memory>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER

__declspec(noalias, restrict ) inline void* aligned_alloc(uint32_t alignment, size_t size)
{
   return _aligned_malloc(size, alignment);
}

inline void aligned_free(void* ptr)
{
   _aligned_free(ptr);
}
#else
inline void aligned_free(void* ptr)
{
   free(ptr);
}
#endif

namespace vstl
{

class MemoryBuffer
{
private:
   static const uint64_t k_size_mask = 0x0FFFFFFFFFFFFFFF;
   static const uint64_t k_log_align_mask  = 0xF000000000000000;
   static const uint64_t k_log_align_shift = 60;
   static const uint64_t k_log_min_alignment = 2;
   static const uint64_t k_max_size = (1ull << k_log_align_shift) - 1ull;
public:
   typedef uint64_t size_type;
   typedef uint32_t alignment_type;

   MemoryBuffer(size_type size, alignment_type alignment = sizeof(size_type))
   {
      init(size, alignment);
   }

   MemoryBuffer(MemoryBuffer const & to_copy)
      : MemoryBuffer(to_copy.size(), to_copy.alignment())
   {
      memcpy(ptr_, to_copy.ptr_, (size_t)size());
   }

   MemoryBuffer(MemoryBuffer && to_move)
      : ptr_ { to_move.ptr_ }
   {
      to_move.ptr_ = nullptr;
   }

   auto operator = (MemoryBuffer const & to_copy) -> MemoryBuffer&
   {
      void* ptr = ptr_;
      if (alignment() != to_copy.alignment() || (size() > to_copy.size()))
      {
         init(to_copy.size(), to_copy.alignment());
      }

      memcpy(ptr, to_copy.ptr_, (size_t)to_copy.size());

      if (ptr != nullptr)
      {
         uint8_t* p = (uint8_t*)ptr_ - preamble_size(alignment());
         aligned_free(p);
         ptr_ = ptr;
      }
      return *this;
   }

   void resize(size_t new_size)
   {
      if (size() > new_size)
      {
         void* ptr = realloc(ptr_, new_size);
         assert(ptr == ptr_);
      }
      else if (size() < new_size)
      {
         void* ptr = aligned_alloc(alignment(), new_size);
         if (ptr == nullptr  && new_size > 0)
         {
            bad_alloc();
         }
         else
         {
            memcpy(ptr, ptr_, new_size);
            uint8_t* p = (uint8_t*)ptr_ - preamble_size(alignment());
            free(p);
            ptr_ = ptr;
         }
      }
      const size_type sz = *size_ptr();
      *size_ptr() = (sz & k_log_align_mask) | (new_size & k_size_mask);
   }

   auto operator = (MemoryBuffer&& to_move) -> MemoryBuffer&
   {
      return *this;
   }

   ~MemoryBuffer()
   {
      if (ptr_ != nullptr)
      {
         uint8_t* p = (uint8_t*)ptr_ - preamble_size(alignment());
         aligned_free(p);
      }
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

   size_type size() const
   {
      return *size_ptr() & k_size_mask;
   }
   alignment_type alignment() const
   {
      size_type sz = *size_ptr() & k_log_align_mask;
      alignment_type log_align = (sz >> k_log_align_shift) + k_log_min_alignment;

      return (alignment_type)(1ull << log_align);
   }
private:
   size_type preamble_size(alignment_type alignment) const
   {
      return sizeof(size_type) > alignment ? sizeof(size_type) : alignment;
   }
   void init(size_type size, alignment_type alignment)
   {
      V_ASSERT(((alignment - 1) & alignment) == 0);
      V_ASSERT(size >= sizeof(size_type));
      V_ASSERT(size <= k_max_size);
	  size_type preamble = preamble_size(alignment);
      void* p = aligned_alloc(alignment, (size_t)(size + preamble));
      if (p == nullptr)
      {
         bad_alloc();
      }
      ptr_ = (uint8_t*)p + preamble;

      size_type size_m = k_size_mask & size;
      size_type* size_position = (size_type*)((uint8_t*)ptr_ - sizeof(size_type));
      uint64_t log_alignment = sizeof(alignment_type) * 8 - __lzcnt(alignment) - 1;
      V_ASSERT(log_alignment < 32);
      *size_position = size_m | ((log_alignment - k_log_min_alignment) << k_log_align_shift);
   }
   void bad_alloc()
   {
#if __cpp_exceptions
      throw (std::bad_alloc());
#else
      V_ASSERT(0);
#endif
   }
   size_type const* size_ptr() const
   {
      return (size_type const*)((uint8_t const*)ptr_ - sizeof(size_type));
   }
   size_type* size_ptr()
   {
	   return (size_type*)((uint8_t*)ptr_ - sizeof(size_type));
   }
   void* ptr_ = nullptr;
};

inline void copy(MemoryBuffer const& src, MemoryBuffer& dst)
{
   V_ASSERT(src.size() <= dst.size());
   memcpy(dst, src, (size_t)src.size());
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