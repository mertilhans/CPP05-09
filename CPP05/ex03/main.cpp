#include "Bureaucrat.hpp"
#include "Intern.hpp"
#include <cstdlib>
#include <ctime>

static void test1()
{
	try
	{
		Intern intern;
		Bureaucrat tst1("Optimus", 1);
		AForm* f1 = intern.makeForm("shrubbery creation", "base");
		if (f1)
		{
			tst1.signForm(*f1);
			tst1.executeForm(*f1);
			delete f1;
		}
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
}

static void test2()
{
	try
	{
		Intern intern;
		Bureaucrat tst2("Bumblebee", 50);
		AForm* f2 = intern.makeForm("robotomy request", "Starscream");
		if (f2)
		{
			tst2.signForm(*f2);
			tst2.executeForm(*f2);
			delete f2;
		}
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
}

static void test3()
{
	try
	{
		Intern intern;
		Bureaucrat tst3("Ironhide", 1);
		AForm* f3 = intern.makeForm("presidential pardon", "Megatron");
		if (f3)
		{
			tst3.signForm(*f3);
			tst3.executeForm(*f3);
			delete f3;
		}
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
}

static void test4()
{
	try
	{
		Intern intern;
		AForm* f4 = intern.makeForm("teleport request", "Soundwave");
		delete f4;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
}

static void test5()
{
	try
	{
		Bureaucrat tst5("Megatron", 1337);
		std::cout << tst5 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "ERror: " << e.what() << std::endl;
	}
}

static void shuffleTests(void (*tests[])(), int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int j = std::rand() % (i + 1);
		void (*tmp)() = tests[i];
		tests[i] = tests[j];
		tests[j] = tmp;
	}
}

int main()
{
	void (*tests[5])() = {test1, test2, test3, test4, test5};

	std::srand(static_cast<unsigned int>(std::time(NULL)));
	shuffleTests(tests, 5);
	for (int i = 0; i < 5; i++)
		tests[i]();

	return 0;
}
