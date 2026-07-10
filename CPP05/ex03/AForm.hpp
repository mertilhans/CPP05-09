#ifndef AFORM_HPP
#define AFORM_HPP

#include <string>
#include <iostream>
#include "Bureaucrat.hpp"

class AForm
{
	private:
		const std::string _name;
		bool _isSigned;
		const int _signGrade;
		const int _executeGrade;

	public:
		AForm();
		AForm(std::string const &name, int signGrade, int executeGrade);
		AForm(AForm const &cpy);
		AForm& operator=(AForm const &cpy);
		virtual ~AForm();

		std::string getName() const;
		bool getIsSigned() const;
		int getSignGrade() const;
		int getExecuteGrade() const;

		void beSigned(Bureaucrat const &k);
		void execute(Bureaucrat const &executor) const;

		class GradeTooHighException : public std::exception
		{
		};
		class GradeTooLowException : public std::exception
		{
		};
		class FormNotSignedException : public std::exception
		{
		};

	protected:
		virtual void executeAction() const = 0;
};

std::ostream& operator<<(std::ostream &op, AForm const &v);

#endif
