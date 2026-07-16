#include "Array.hpp"
#include <cstddef>
#include <exception>

template <typename t>


Array<t>::Array() : _data(NULL) , _len(0) {}

template <typename t>

Array<t>::Array(unsigned int n) :  _data(new t[n]()), _len(n) {}

template <typename t>

Array<t>::Array(const Array &cpy) : _data(new t[cpy._len]) , _len(cpy._len) {
	unsigned int i = -1;
	while(++i < _len)
		_data[i] = cpy._data[i];
}

template <typename t>

Array<t>& Array<t>::operator=(const Array &cpy)
{
	if (this != &cpy)
	{
		delete[] _data;
		_data = new t[cpy._len];
		for (unsigned int i = 0; i < cpy._len; i++)
			_data[i] = cpy._data[i];
		_len = cpy._len;
	}
	return *this;
}
template <typename t>

t& Array<t>::operator[] (int i)
{
	return(i < 0 || (unsigned int)i >= _len) ? throw std::exception() : _data[i];
}
template <typename t>

Array<t>::~Array()
{
	delete[] _data;
}
template <typename t>

unsigned int Array<t>::size() const
{
	return _len;
}



