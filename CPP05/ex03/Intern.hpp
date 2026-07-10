#ifndef INTERN_HPP
#define INTERN_HPP

#include "AForm.hpp"

class Intern
{
	public:
		Intern();
		Intern(Intern const &cpy);
		Intern& operator=(Intern const &cpy);
		~Intern();

		AForm* makeForm(std::string const &formName, std::string const &target);
};

#endif
