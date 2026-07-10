#include "AForm.hpp"

AForm::AForm() : _name("AForm"), _isSigned(false), _signGrade(150), _executeGrade(150)
{
}
AForm::AForm(AForm const &cpy) : _name(cpy._name), _isSigned(cpy._isSigned), _signGrade(cpy._signGrade), _executeGrade(cpy._executeGrade)
{
}
AForm::AForm(std::string const &name, int signGrade, int executeGrade) : _name(name), _isSigned(false), _signGrade(signGrade), _executeGrade(executeGrade)
{
	if (signGrade < 1 || executeGrade < 1)
		throw AForm::GradeTooHighException();
	if (signGrade > 150 || executeGrade > 150)
		throw AForm::GradeTooLowException();
}
AForm& AForm::operator=(AForm const &cpy)
{
	if (this != &cpy)
		_isSigned = cpy._isSigned;
	return (*this);
}
AForm::~AForm()
{
}
std::string AForm::getName() const
{
	return (_name);
}
bool AForm::getIsSigned() const
{
	return (_isSigned);
}
int AForm::getSignGrade() const
{
	return (_signGrade);
}
int AForm::getExecuteGrade() const
{
	return (_executeGrade);
}
void AForm::beSigned(Bureaucrat const &k)
{
	if (k.getGrade() > _signGrade)
		throw AForm::GradeTooLowException();
	_isSigned = true;
}
void AForm::execute(Bureaucrat const &executor) const
{
	if (!_isSigned)
		throw AForm::FormNotSignedException();
	if (executor.getGrade() > _executeGrade)
		throw AForm::GradeTooLowException();
	executeAction();
}
std::ostream& operator<<(std::ostream &op, AForm const &v)
{
	op << v.getName() << ", (signed): " << v.getIsSigned() << ", sign grade " << v.getSignGrade() << ", execute grade " << v.getExecuteGrade() << ".";
	return (op);
}
