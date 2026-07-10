#include "RobotomyRequestForm.hpp"
#include <cstdlib>
#include <ctime>

RobotomyRequestForm::RobotomyRequestForm(std::string const &target) : AForm("RobotomyRequestForm", 72, 45), _target(target)
{
	std::srand(std::time(NULL));
}
RobotomyRequestForm::RobotomyRequestForm(RobotomyRequestForm const &cpy) : AForm(cpy), _target(cpy._target)
{
}
RobotomyRequestForm& RobotomyRequestForm::operator=(RobotomyRequestForm const &cpy)
{
	if (this != &cpy)
		AForm::operator=(cpy);
	return (*this);
}
RobotomyRequestForm::~RobotomyRequestForm()
{
}
void RobotomyRequestForm::executeAction() const
{
	std::cout << "* DRILLING NOISES *" << std::endl;
	if (std::rand() % 2 == 0)
		std::cout << _target << " has been robotomized successfully" << std::endl;
	else
		std::cout << "Robotomy of " << _target << " failed" << std::endl;
}
