#include "Bureaucrat.hpp"
#include "AForm.hpp"

Bureaucrat::Bureaucrat() : _name("Bureaucrat") , _grade(13)
{
}
Bureaucrat::Bureaucrat(Bureaucrat const &cpy) : _name(cpy._name), _grade(cpy._grade)
{
}
Bureaucrat::Bureaucrat(std::string const & name, int grade) : _name(name), _grade(grade)
{
	if (grade < 1)
		throw Bureaucrat::GradeTooHighException(); 
	if (grade > 150)
		throw Bureaucrat::GradeTooLowException(); 
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
		throw Bureaucrat::GradeTooHighException();
	_grade = _grade - 1;
}
void Bureaucrat::decrementGrade()
{
	if (_grade + 1 > 150)
		throw Bureaucrat::GradeTooLowException();
	_grade = _grade + 1;
}
void Bureaucrat::signForm(AForm &form)
{
	try
	{
		form.beSigned(*this);
		std::cout << _name << " signed " << form.getName() << std::endl;
	}
	catch (AForm::GradeTooHighException &e)
	{
		std::cout << _name << " couldn't sign " << form.getName() << " because grade is too high" << std::endl;
	}
	catch (AForm::GradeTooLowException &e)
	{
		std::cout << _name << " couldn't sign " << form.getName() << " because grade is too low" << std::endl;
	}
}
void Bureaucrat::executeForm(AForm const &form) const
{
	try
	{
		form.execute(*this);
		std::cout << _name << " executed " << form.getName() << std::endl;
	}
	catch (AForm::FormNotSignedException &e)
	{
		std::cout << _name << " couldn't execute " << form.getName() << " because form is not signed" << std::endl;
	}
	catch (AForm::GradeTooHighException &e)
	{
		std::cout << _name << " couldn't execute " << form.getName() << " because grade is too high" << std::endl;
	}
	catch (AForm::GradeTooLowException &e)
	{
		std::cout << _name << " couldn't execute " << form.getName() << " because grade is too low" << std::endl;
	}
}
std::ostream& operator<<(std::ostream& op, Bureaucrat const &b)
{
	op << b.getName() << ", bureaucrat grade " << b.getGrade() << ".";
	return (op);
}
