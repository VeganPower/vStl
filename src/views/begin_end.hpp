#pragma once
#include "iterator.hpp"

namespace vStl
{
// Defines begin() end() cbegin() cend() rbegin() rend() rcbegin() rcend()
template<bool is_reverse, bool is_const>
struct begin_impl
{
template<typename View>
constexpr iterator<Cursor> operator()(View const& v)
{

}

};

};