#ifndef ROBOTOMYREQUESTFORM_HPP
#define ROBOTOMYREQUESTFORM_HPP

#include "AForm.hpp"

class RobotomyRequestForm : public AForm
{
	public:
		RobotomyRequestForm(std::string const &target);
		RobotomyRequestForm(RobotomyRequestForm const &cpy);
		RobotomyRequestForm& operator=(RobotomyRequestForm const &cpy);
		~RobotomyRequestForm();

	protected:
		virtual void executeAction() const;

	private:
		const std::string _target;
};

#endif
