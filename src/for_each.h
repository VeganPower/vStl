#pragma once

namespace vstl
{

template<typename Range_t, typename Fun_t>
void for_each(Range_t const& r, Fun_t f)
{
   typename Range_t::iterator it = r.begin();
   typename Range_t::iterator_end end = r.end();
   while(it != end)
   {
      f(*it);
      // TODO: Should I just use operator++?
      Range_t::advance(it);
   }
}

}
