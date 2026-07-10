#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <string>
#include <iostream>

class AForm;
class Bureaucrat
{
	private:
		const std::string _name;
		int _grade;
	public:
		Bureaucrat(); 
		Bureaucrat(std::string const &name, int grade);
		Bureaucrat(Bureaucrat const &cpy);
		Bureaucrat& operator=(Bureaucrat const &cpy);
		~Bureaucrat();

		std::string getName() const;
		int getGrade() const;

		void incrementGrade();
		void decrementGrade();
		class GradeTooHighException : public std::exception
		{
		};
		class GradeTooLowException : public std::exception
		{
		};
		void signForm(AForm &form);
		void executeForm(AForm const &form) const;

};
std::ostream& operator<<(std::ostream& op ,Bureaucrat const &b);
#endif
