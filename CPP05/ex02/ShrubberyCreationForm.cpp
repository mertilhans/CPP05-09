#include "ShrubberyCreationForm.hpp"
#include <fstream>

ShrubberyCreationForm::ShrubberyCreationForm(std::string const &target) : AForm("ShrubberyCreationForm", 145, 137), _target(target)
{
}
ShrubberyCreationForm::ShrubberyCreationForm(ShrubberyCreationForm const &cpy) : AForm(cpy), _target(cpy._target)
{
}
ShrubberyCreationForm& ShrubberyCreationForm::operator=(ShrubberyCreationForm const &cpy)
{
	if (this != &cpy)
		AForm::operator=(cpy);
	return (*this);
}
ShrubberyCreationForm::~ShrubberyCreationForm()
{
}
void ShrubberyCreationForm::executeAction() const
{
	std::ofstream file((_target + "_shrubbery").c_str());
// turn this 
	file << "       *\n";
	file << "      ***\n";
	file << "     *****\n";
	file << "    *******\n";
	file << "   *********\n";
	file << "  ***********\n";
	file << " *************\n";
	file << "       |\n";
	file.close();
}
