#ifndef ARRAY_HPP
#define ARRAY_HPP

template <typename t>
class Array
{
	private:
		t *_data;
		unsigned int _len;
	public:
		Array();
		Array(unsigned int n);
		Array(const Array &cpy);
		Array& operator=(const Array &cpy);
		t& operator[](int i);
		~Array();
		unsigned int size() const;

};
#include "Array.tpp"

#endif
