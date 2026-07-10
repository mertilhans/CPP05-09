#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main()
{
	try
	{
		Bureaucrat tst1("Soap", 1);
		ShrubberyCreationForm f1("garden");
		std::cout << f1 << std::endl;
		tst1.signForm(f1);
		tst1.executeForm(f1);
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
	try
	{
		Bureaucrat tst2("Ghost", 100);
		RobotomyRequestForm f2("Bender");
		tst2.executeForm(f2);
		tst2.signForm(f2);
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
	try
	{
		Bureaucrat tst3("Price", 1);
		RobotomyRequestForm f3("Fry");
		tst3.signForm(f3);
		tst3.executeForm(f3);
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
	try
	{
		Bureaucrat tst4("Gaz", 1);
		PresidentialPardonForm f4("Leela");
		tst4.signForm(f4);
		tst4.executeForm(f4);
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
	try
	{
		Bureaucrat tst5("Roach", 1);
		ShrubberyCreationForm f5("base");
		tst5.signForm(f5);
		ShrubberyCreationForm f6(f5);
		std::cout << f6 << std::endl;
		tst5.executeForm(f6);
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
	try
	{
		Bureaucrat tst6("Yuri", 150);
		PresidentialPardonForm f7("Makarov");
		tst6.signForm(f7);
		tst6.executeForm(f7);
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
	try
	{
		Bureaucrat tst7("Shepherd", 1337);
		std::cout << tst7 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}

	return 0;
}
