#include "Form.hpp"
#include <sstream>

Form::Form() : _name("Form"), _isSigned(false), _signGrade(150), _executeGrade(150)
{
}
Form::Form(Form const &cpy) : _name(cpy._name), _isSigned(cpy._isSigned), _signGrade(cpy._signGrade), _executeGrade(cpy._executeGrade)
{
}
Form::Form(std::string const & name, int signGrade, int executeGrade) : _name(name), _isSigned(false), _signGrade(signGrade), _executeGrade(executeGrade)
{
	if (signGrade < 1 || executeGrade < 1)
	{
		std::ostringstream oss;
		oss << "form \"" << name << "\" needs a grade >= 1 (got sign=" << signGrade << ", execute=" << executeGrade << ")";
		throw Form::GradeTooHighException(oss.str());
	}
	if (signGrade > 150 || executeGrade > 150)
	{
		std::ostringstream oss;
		oss << "form \"" << name << "\" needs a grade <= 150 (got sign=" << signGrade << ", execute=" << executeGrade << ")";
		throw Form::GradeTooLowException(oss.str());
	}
}
Form& Form::operator=(Form const &cpy)
{
	if (this != &cpy)
		_isSigned = cpy._isSigned;
	return *this;
}
Form::~Form()
{
}
std::string Form::getName() const
{
	return(_name);
}
bool Form::getIsSigned() const
{
	return(_isSigned);
}
int Form::getSignGrade() const
{
	return(_signGrade);
}
int Form::getExecuteGrade() const
{
	return(_executeGrade);
}
void Form::beSigned(Bureaucrat const &k)
{
	if (k.getGrade() > _signGrade)
	{
		std::ostringstream oss;
		oss << k.getName() << " (grade " << k.getGrade() << ") is too low to sign \"" << _name << "\" (requires grade <= " << _signGrade << ")";
		throw Form::GradeTooLowException(oss.str());
	}
	_isSigned = true;
}
Form::GradeTooHighException::GradeTooHighException(std::string const &msg) : _msg(msg)
{
}
Form::GradeTooHighException::~GradeTooHighException() throw()
{
}
const char *Form::GradeTooHighException::what() const throw()
{
	return (_msg.c_str());
}
Form::GradeTooLowException::GradeTooLowException(std::string const &msg) : _msg(msg)
{
}
Form::GradeTooLowException::~GradeTooLowException() throw()
{
}
const char *Form::GradeTooLowException::what() const throw()
{
	return (_msg.c_str());
}
std::ostream& operator<<(std::ostream &op, Form const& v)
{
	op << v.getName() << ", (signed):  " << v.getIsSigned() << ", sign grade :   " << v.getSignGrade() << ", execute grade :    " << v.getExecuteGrade() << ".";
	return (op);
}
