#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <sstream>

Bureaucrat::Bureaucrat() : _name("Bureaucrat") , _grade(13)
{
}
Bureaucrat::Bureaucrat(Bureaucrat const &cpy) : _name(cpy._name), _grade(cpy._grade)
{
}
Bureaucrat::Bureaucrat(std::string const & name, int grade) : _name(name), _grade(grade)
{
	if (grade < 1)
		throw Bureaucrat::GradeTooHighException(grade);
	if (grade > 150)
		throw Bureaucrat::GradeTooLowException(grade);
}
Bureaucrat& Bureaucrat::operator=(Bureaucrat const &cpy)
{
	if(this != &cpy)
		_grade = cpy._grade;
	return *this;
}
Bureaucrat::~Bureaucrat()
{
}
std::string Bureaucrat::getName() const
{
	return(_name);
}
int Bureaucrat::getGrade() const
{
	return(_grade);
}
void	Bureaucrat::incrementGrade()
{
	if (_grade -1 < 1)
		throw Bureaucrat::GradeTooHighException(_grade - 1);
	_grade = _grade - 1;
}
void Bureaucrat::decrementGrade()
{
	if (_grade + 1 > 150)
		throw Bureaucrat::GradeTooLowException(_grade + 1);
	_grade = _grade + 1;
}
void Bureaucrat::signForm(Form &form)
{
	try
	{
		form.beSigned(*this);
		std::cout << _name << " signed " << form.getName() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << _name << " couldn't sign " << form.getName() << " because " << e.what() << std::endl;
	}
}
Bureaucrat::GradeTooHighException::GradeTooHighException(int grade)
{
	std::ostringstream oss;
	oss << "grade " << grade << " is too high for a bureaucrat (min is 1)";
	_msg = oss.str();
}
Bureaucrat::GradeTooHighException::~GradeTooHighException() throw()
{
}
const char *Bureaucrat::GradeTooHighException::what() const throw()
{
	return (_msg.c_str());
}
Bureaucrat::GradeTooLowException::GradeTooLowException(int grade)
{
	std::ostringstream oss;
	oss << "grade " << grade << " is too low for a bureaucrat (max is 150)";
	_msg = oss.str();
}
Bureaucrat::GradeTooLowException::~GradeTooLowException() throw()
{
}
const char *Bureaucrat::GradeTooLowException::what() const throw()
{
	return (_msg.c_str());
}
std::ostream& operator<<(std::ostream& op, Bureaucrat const &b)
{
	op << b.getName() << ", bureaucrat grade " << b.getGrade() << ".";
	return (op);
}
