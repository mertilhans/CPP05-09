#include "Intern.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

Intern::Intern()
{
}
Intern::Intern(Intern const &cpy)
{
	(void)cpy;
}
Intern& Intern::operator=(Intern const &cpy)
{
	(void)cpy;
	return (*this);
}
Intern::~Intern()
{
}

static AForm* createShrubbery(std::string const &target)
{
	return (new ShrubberyCreationForm(target));
}
static AForm* createRobotomy(std::string const &target)
{
	return (new RobotomyRequestForm(target));
}
static AForm* createPardon(std::string const &target)
{
	return (new PresidentialPardonForm(target));
}

AForm* Intern::makeForm(std::string const &formName, std::string const &target)
{
	std::string names[3] = {"shrubbery creation", "robotomy request", "presidential pardon"};
	AForm* (*funcs[3])(std::string const &) = {createShrubbery, createRobotomy, createPardon};

	for (int i = 0; i < 3; i++)
	{
		if (formName == names[i])
		{
			std::cout << "Intern creates " << formName << std::endl;
			return (funcs[i](target));
		}
	}
	std::cout << "Intern couldn't find a form named " << formName << std::endl;
	return (NULL);
}
