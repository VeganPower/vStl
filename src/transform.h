// transform

namespace vstl
{


template<typename Base, typename Func>
struct TransformRange
{
   struct Iterator
   {
   private:
      typedef 
      Func func_;
      decltype( begin(std::declval<Base&>())) it_;
   public:
      decltype(auto) operator *() const
      {
         return func_(*it_);
      }
      decltype(auto) base() const
      {
         return it_;
      }
   };
};

};