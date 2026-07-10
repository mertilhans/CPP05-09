#ifndef FORM_HPP
#define FORM_HPP

#include <string>
#include <iostream>
#include "Bureaucrat.hpp"

class Form
{
	private:
		const std::string _name;
		bool _isSigned;
		const int _signGrade;
		const int _executeGrade;

	public:
		Form();
		Form(std::string const &name, int signGrade,int executeGrade);
		Form(Form const &cpy);
		Form& operator=(Form const &cpy);
		~Form();

		std::string getName() const;
		bool getIsSigned() const;
		int getSignGrade() const;
		int getExecuteGrade() const;
		void beSigned(Bureaucrat const &k);


		class GradeTooHighException : public std::exception
		{
			private:
				std::string _msg;
			public:
				GradeTooHighException(std::string const &msg);
				~GradeTooHighException() throw();
				const char *what() const throw();
		};
		class GradeTooLowException : public std::exception
		{
			private:
				std::string _msg;
			public:
				GradeTooLowException(std::string const &msg);
				~GradeTooLowException() throw();
				const char *what() const throw();
		};
};

std::ostream& operator<<(std::ostream &op,Form const& v);



#endif