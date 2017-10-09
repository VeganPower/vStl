//

#include "range.h"
// filter
//  


//->filter(it)
  //->take(3)


/// LAZY EVALUATION
// Keep reference for l-value
// Copy container for r-value

// BAAAD
// TODO:
// adaptor object carry everything that is common to all iterator (func/ end)
// iterator carry reference to adaptor
#if 0
template<typename Base, typename Func>
struct FiltredRange
{
   Base& base_;
   Func func_;
/*
   using Index = typename Base::Index;
   void increment_index(Index& idx)
   {
      do {
         base_.increment_index(idx);
      } while (idx != base_.end_index()
               && !func_(base_.dereference_index(idx))
         );
   }
*/
   struct Iterator
   {
   private:
      decltype( begin(std::declval<Base&>())) it_;
      // This should go in adator
      decltype( begin(std::declval<Base&>())) it_end_;
      Func func_;
   public:
      iterator& operator++()
      {
         do {
            ++it_;
         }
         while( it_ != it_end_ && !static_cast<bool>(func_(*it)))
         return *this;
      }
   }; 
};
#endif

namespace vstl
{

template<typename Base, typename Func_t>
struct FiltredRange : Base
{
   // typedef Base::iterator iterator;
   // typedef Base::iterator_end iterator_end;
   
   Func_t f_;

   FiltredRange(Base const& b, Func_t f)
      : Base { b }
      , f_ { f }
   {
   }

   typename Base::iterator begin() const
   {
      typename Base::iterator it = Base::begin();
      if(!f_(Base::get(it)))
      {
         advance(it);
      }
      return it;
   }

   typename Base::iterator_end end() const
   {
      Base::end();
   }

   static typename Base::type get(typename Base::iterator const& it)
   {
      return Base::get(it);
   }

   static bool advance(typename Base::iterator& it)
   {
      do
      {
         if (Base::compare(it, Base::end()))
         {
            false;
         }
         Base::advance(it);
      }
      while(!f_(*it));  // Fix me: This one should stop if iterator reach the end of the range. :-/  
      return true;
   }
};

// template<typename iterator, typename func_t>
// bool operator!=(FilteredIterator<iterator, func_t> const& lhs, iterator const& rhs)
// {
//    return lhs.it != rhs;
// }

// template<typename iterator, typename func_t>
// bool operator!=(iterator const& lhs, FilteredIterator<iterator, func_t> const& rhs)
// {
//    return lhs != rhs.it;
// }


template<typename T, typename Func_t>
auto filter(T const& base, Func_t const& f) // -> Range<FilteredIterator<typename T::>
{
   // typedef FiltredRange<T, Func_t> it_t;
   // it_t it { base.begin(), f };
   return FiltredRange<T, Func_t>(base, f);
}

#if 0
template<typename Base, typename Func>
struct IndexRange
{
   using Index = ...;
   Index begin_index() const;
   Index end_index() const;
   void increment_index(Index &idx) const;
   void decrement_index(Index &idx) const;
   Reference dereference_index(Index &idx) const;
};

template<typename IndexRng>
struct IteratorForIndex
{
   IndexRng rng_;
   typename IndexRng::Index idx_;

   iterator& operator++()
   {
      rnd_.increment_index(idx_);
      return *this;
   }
};
#endif
}