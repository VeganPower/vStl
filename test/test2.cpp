#include "static_array.h"
#include <iostream>
#include <functional>

struct IntView
{
   int* cursor = nullptr;
   int* sentinel = nullptr;
};

template<typename View_t>
struct Filter
{
   Filter(View_t const& v, std::function<bool(int)> f);

   View_t view;
   std::function<bool(int)> pred;
};

template<typename View_t>
struct Transform
{
   Transform(View_t const&v, std::function<int(int)> p)
      : view(v)
      , pred(p)
   {}
   View_t view;
   std::function<int(int)> pred;
};

template<typename View_t>
struct Take
{
   Take(View_t const& v, int c)
      : view(v)
      , count(c)
   {}
   View_t view;      
   int count = 0;
};

template<typename View_t>
Filter<View_t>::Filter(View_t const& v, std::function<bool(int)> f)
   : view(v)
   , pred(f)
{
   // this require a "write" function
   view.cursor = advance_to_first_valid(*this);   
}

template<typename View_t>
int* advance_to_first_valid(Filter<View_t>& f)
{
   while(!done(f) && !f.pred(read(f.view)))
   {
      next(f.view);
   }
}

void next(IntView& v)
{
   v.cursor++;
}

template<typename View_t>
void next(Filter<View_t>& f)
{
   next(f.view);
   advance_to_first_valid(f);
}

template<typename View_t>
void next(Transform<View_t>& t)
{
   // nothing to do
   next(t.view);
}

template<typename View_t>
void next(Take<View_t>& t)
{
   t.count--;
   next(t.view);
}

template<typename View_t>
int read(Filter<View_t> const& t)
{
   return *t.view.cursor;
}

template<typename View_t>
int read(Transform<View_t> const& t)
{
   return t.pred(read(t.view));
}

template<typename View_t>
int read(Take<View_t>const& t)
{
   return read(t.view);
}

bool done(IntView const& v)
{
   return v.cursor == v.sentinel;
}

template<typename View_t>
bool done(Filter<View_t> const& f)
{
   return done(f.view);
}

template<typename View_t>
bool done(Transform<View_t> const& t)
{
   // nothing to add
   return done(t.view);
}

template<typename View_t>
bool done(Take<View_t> const& t)
{
   return (t.count == 0) || done(t.view);
}

int main(int argc, char const *argv[])
{
   vstl::StaticArray<int> a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

   // a.iter() | filter(x>3) | transform(x*x) | take(4)
   IntView view = { &a[0], &a[a.size()] };
   for (Take t(Transform(Filter(view, [](int x)
      {
         return x > 3;
      }), [](int x) -> int
      {
         return x*x;
      }), 4); !done(t); view.cursor = next(t))
   {
      int const& x = read(t);
      std::cout << x << " ";
   }
   std::cout << std::endl;
   return 0;
}