#ifndef SHRUBBERYCREATIONFORM_HPP
#define SHRUBBERYCREATIONFORM_HPP

#include "AForm.hpp"

class ShrubberyCreationForm : public AForm
{
	public:
		ShrubberyCreationForm(std::string const &target);
		ShrubberyCreationForm(ShrubberyCreationForm const &cpy);
		ShrubberyCreationForm& operator=(ShrubberyCreationForm const &cpy);
		~ShrubberyCreationForm();

	protected:
		virtual void executeAction() const;

	private:
		const std::string _target;
};

#endif
