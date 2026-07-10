#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <string>
#include <iostream>

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
			private:
				std::string _msg;
			public:
				GradeTooHighException(int grade);
				~GradeTooHighException() throw();
				const char *what() const throw();
		};
		class GradeTooLowException : public std::exception
		{
			private:
				std::string _msg;
			public:
				GradeTooLowException(int grade);
				~GradeTooLowException() throw();
				const char *what() const throw();
		};
};
std::ostream& operator<<(std::ostream& op ,Bureaucrat const &b);
#endif
