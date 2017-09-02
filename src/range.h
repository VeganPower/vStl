#include <type_traits>

namespace vstl
{

template<typename T>
class Range
{
public:
   typedef typename std::remove_const<T>::type Type;
   typedef Type* Ptr;
   typedef const Type* ConstPtr;
   Range(Ptr b, Ptr e)
      : b_(b)
      , e_(e)
   {}
   Ptr begin() const { return b_; }
   Ptr end() const { return e_; }
private:
   const Ptr b_;
   const Ptr e_;
};

template<typename T>
class ConstRange
{
public:
   typedef typename std::remove_const<T>::type Type;
   typedef const Type* ConstPtr;

   ConstRange(ConstPtr a, ConstPtr b);
   ConstPtr begin() const { return b_; }
   ConstPtr end() const { return e_; }
private:
   ConstPtr b_;
   ConstPtr e_;
};


}