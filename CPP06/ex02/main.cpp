#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

Base* generate(void)
{
	int rnd = std::rand() % 3;

	if(rnd == 0)
		return new A();
	else if(rnd == 1)
		return new B();
	else
		return new C();
}

void identify(Base* obj)
{
	if(dynamic_cast<A*>(obj))
		std::cout << "A" << std::endl;
	else if(dynamic_cast<B*>(obj))
		std::cout << "B" << std::endl;
	else if(dynamic_cast<C*>(obj))
		std::cout << "C" << std::endl;
	else
		std::cout << "Unknown" << std::endl;
}

void identify(Base& obj)
{
	try
	{
		(void)dynamic_cast<A&>(obj);
		std::cout << "A" << std::endl;
		return;
	}
	catch(...) {}

	try
	{
		(void)dynamic_cast<B&>(obj);
		std::cout << "B" << std::endl;
		return;
	}
	catch(...) {}

	try
	{
		(void)dynamic_cast<C&>(obj);
		std::cout << "C" << std::endl;
		return;
	}
	catch(...) {}

	std::cout << "Unknown" << std::endl;
}

int main(void)
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	int i = 0;
	while(i < 5)
	{
		Base *base = generate();

		std::cout << "--- case " << i << " ---" << std::endl;
		std::cout << "identify(Base*):  ";
		identify(base);

		std::cout << "identify(Base&):  ";
		identify(*base);

		delete base;
		i++;
	}
	return 0;
}
