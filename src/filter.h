//

// filter
//  

/// LAZY EVALUATION
// Keep reference for l-value
// Copy container for r-value

// BAAAD
// TODO:
// adaptor object carry everything that is common to all iterator (func/ end)
// iterator carry reference to adaptor
template<typename Base, typename Func>
struct FilterRange
{
   Func func_;
   Base& base_;

   using Index = typename Base::Index;
   void increment_index(Index& idx)
   {
      do {
         base_.increment_index(idx);
      } while (idx != base_.end_index()
               && !func_(base_.dereference_index(idx))
         );
   }
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
