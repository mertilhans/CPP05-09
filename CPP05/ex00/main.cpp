#include "Bureaucrat.hpp"

int main()
{
	try
	{
		Bureaucrat tst("test",0);
		std::cout << tst << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror" << std::endl;
	}
	try
	{
		Bureaucrat tst1("Nasha",1);
		std::cout << tst1 << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	try
	{
		Bureaucrat tst2("Katya",150);
		std::cout << tst2 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror" << std::endl;
	}
	try
	{
		Bureaucrat tst3("alex",1337);
		std::cout << tst3 << std::endl;
	}
	catch (...)
	{
		std::cout << "ERror" << std::endl;
	}
	try
	{
		Bureaucrat tst4("Svetlana",13);
		std::cout << tst4 << std::endl;
		tst4.incrementGrade();
		std::cout << tst4 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERoor" << std::endl;
	}
	try
	{
		Bureaucrat tst5("Anastasia",150);
		std::cout << tst5 << std::endl;
		tst5.decrementGrade();
		std::cout << tst5 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror" << std::endl;
	}

	return 0;
}