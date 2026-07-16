#ifndef ITER_HPP
#define ITER_HPP
#include <iostream>

template <typename t, typename func>


void iter(t *array, const unsigned int len, func f)
{
	if(!array)
		return;
	size_t i = -1;
	while(++i < len)
		f(array[i]);
}
template <typename t>

void print(t &p)
{
	std::cout << p << std::endl;
}
template <typename t>

t increment(t &i)
{
	i += 1;
	return i;
}


#endif
