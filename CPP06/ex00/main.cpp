#include "ScalarConverter.hpp"
#include <iostream>

int main(int ac,char **av)
{
	if(ac != 2)
		std::cerr << "ERROR" << std::endl;
	else
		ScalarConverter::convert(av[1]);
	return 0;

}