#ifndef PRESIDENTIALPARDONFORM_HPP
#define PRESIDENTIALPARDONFORM_HPP

#include "AForm.hpp"

class PresidentialPardonForm : public AForm
{
	public:
		PresidentialPardonForm(std::string const &target);
		PresidentialPardonForm(PresidentialPardonForm const &cpy);
		PresidentialPardonForm& operator=(PresidentialPardonForm const &cpy);
		~PresidentialPardonForm();

	protected:
		virtual void executeAction() const;

	private:
		const std::string _target;
};

#endif
