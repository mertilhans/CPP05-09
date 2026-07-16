#include <iostream>
#include "Array.hpp"


int main(int, char**)
{
	Array<int> empty;
	std::cout << "empty size = " << empty.size() << std::endl;
	std::cout << "-----------------------" << std::endl;

	Array<int>  pcc(5);
	std::cout << "pcc size =  " << pcc.size() << std::endl;
	std::cout << "-----------------------" << std::endl;

	pcc[0] = 42;
	pcc[4] = 1337;
	std::cout << "[0] == " << pcc[0] << "[4]. == " << pcc[4] << std::endl;
	std::cout << "-----------------------" << std::endl;


	Array<int> copy0fpcc(pcc);
	copy0fpcc[0] = 1337;

	std::cout << "pcc[0] = " << pcc[0] << "its not changed " << std::endl;
	std::cout << "copyOfpcc[0] = " << copy0fpcc[0] << std::endl;
		std::cout << "-----------------------" << std::endl;


Array<int> a(13);
Array<int> b(37);
a[0] = 777;
b = a;
b[0] = 222;
std::cout << "a[0] = " << a[0] << " (its not changed)" << std::endl;
std::cout << "b[0] = " << b[0] << std::endl;
std::cout << "b.size() = " << b.size() << " (should be same as a.size(): " << a.size() << ")" << std::endl;
std::cout << "-----------------------" << std::endl;

	try
	{
		pcc[100] = 1;
	}
	catch (const std::exception &e)
	{
		std::cout << "out of bounds caught: " << e.what() << std::endl;
	}

	try
	{
		pcc[-1] = 1;
	}
	catch (const std::exception &e)
	{
		std::cout << "negative index caught: " << e.what() << std::endl;
	}
	std::cout << "-----------------------" << std::endl;

	return 0;






}
