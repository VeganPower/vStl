#pragma once

namespace vstl
{

template<typename View, typename Fun_t>
void for_each(View& r, Fun_t f)
{
   auto it = r.begin();
   auto end = r.end();
   while(it != end)
   {
      f(*it);
      ++it;
   }
}

}
