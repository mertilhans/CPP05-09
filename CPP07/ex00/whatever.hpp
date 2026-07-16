#ifndef WHATEVER_HPP
#define WHATEVER_HPP
#include <iostream>


template <typename Temp>

void swap(Temp &x,Temp &y)
{
	Temp tmp = x;
	x = y;
	y = tmp;
}
template <typename t>

const t &max(t &x, t&y)
{
	return(x > y ? x : y);
}
template <typename f>

const f &min(f &x, f &y)
{
	return(x < y ? x : y);
}
#endif
