#include "PresidentialPardonForm.hpp"

PresidentialPardonForm::PresidentialPardonForm(std::string const &target) : AForm("PresidentialPardonForm", 25, 5), _target(target)
{
}
PresidentialPardonForm::PresidentialPardonForm(PresidentialPardonForm const &cpy) : AForm(cpy), _target(cpy._target)
{
}
PresidentialPardonForm& PresidentialPardonForm::operator=(PresidentialPardonForm const &cpy)
{
	if (this != &cpy)
		AForm::operator=(cpy);
	return (*this);
}
PresidentialPardonForm::~PresidentialPardonForm()
{
}
void PresidentialPardonForm::executeAction() const
{
	std::cout << _target << " has been pardoned by Zaphod Beeblebrox" << std::endl;
}
