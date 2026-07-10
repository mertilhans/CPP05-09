#include "Bureaucrat.hpp"
#include "Form.hpp"

int main()
{
	try
	{
		Form f1("test", 0, 1);
		std::cout << f1 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror" << std::endl;
	}
	try
	{
		Bureaucrat tst1("Nasha", 1);
		Form f2("passport", 50, 100);
		std::cout << f2 << std::endl;
		tst1.signForm(f2);
		std::cout << f2 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	try
	{
		Bureaucrat tst2("Katya", 100);
		Form f3("visa", 50, 100);
		tst2.signForm(f3);
		std::cout << f3 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror" << std::endl;
	}
	try
	{
		Form f4("license", 200, 1);
		std::cout << f4 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror" << std::endl;
	}
	try
	{
		Bureaucrat tst4("Svetlana", 13);
		Form f5("contract", 13, 13);
		tst4.signForm(f5);
		std::cout << f5 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERoor" << std::endl;
	}
	try
	{
		Bureaucrat tst5("Anastasia", 150);
		Form f6("permit", 1, 1);
		tst5.signForm(f6);
		std::cout << f6 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror" << std::endl;
	}
	try
	{
		Bureaucrat tst6("Irina", 5);
		Form f7("diploma", 10, 10);
		tst6.signForm(f7);
		std::cout << f7 << std::endl;
		tst6.signForm(f7);
		std::cout << f7 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror" << std::endl;
	}
	try
	{
		Bureaucrat tst7("Olga", 20);
		Form f8("driver license", 20, 20);
		tst7.signForm(f8);
		Form f9(f8);
		std::cout << f9 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror" << std::endl;
	}
	try
	{
		Bureaucrat tst8("Nasha", 1);
		Bureaucrat tst9("Olga", 20);
		tst9 = tst8;
		std::cout << tst9 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror" << std::endl;
	}

	return 0;
}
