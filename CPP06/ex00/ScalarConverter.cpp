#include "ScalarConverter.hpp"
#include <cctype>
#include <iostream>
#include <climits>
#include <cstdlib>
#include <cmath>

bool ScalarConverter::isIntLiteral(const std::string &literal)
{
	size_t i = 0;
	if(!literal.size())
		return false;
	if(literal[0] == '+' || literal[0] == '-')
		i = 1;
	if(i == literal.size())
		return false;
	while(i < literal.size())
	{
		if(!std::isdigit(static_cast<unsigned char>(literal[i])))
			return false;
		i++;
	}
	return true;
}
bool ScalarConverter::isFloatLiteral(const std::string &literal)
{
	if (literal == "nanf" || literal == "+inff" || literal == "-inff")
		return true;
	if (literal.empty() || literal[literal.size() - 1] != 'f')
		return false;

	std::string body = literal.substr(0, literal.size() - 1);
	size_t i = 0;
	bool isDigit = false;
	bool isDot = false;

	if (body.empty())
		return false;
	if (body[0] == '+' || body[0] == '-')
		i = 1;
	while (i < body.size())
	{
		if (body[i] == '.' && !isDot)
			isDot = true;
		else if (std::isdigit(static_cast<unsigned char>(body[i])))
			isDigit = true;
		else
			return false;
		i++;
	}
	return isDigit;
}

bool ScalarConverter::isDoubleLiteral(const std::string &literal)
{
	if (literal == "nan" || literal == "+inf" || literal == "-inf")
		return true;

	size_t i = 0;
	bool isDigit = false;
	bool isDot = false;

	if (literal.empty())
		return false;
	if (literal[0] == '+' || literal[0] == '-')
		i = 1;
	while (i < literal.size())
	{
		if (literal[i] == '.' && !isDot)
			isDot = true;
		else if (std::isdigit(static_cast<unsigned char>(literal[i])))
			isDigit = true;
		else
			return false;
		i++;
	}
	return isDigit && isDot;
}


void ScalarConverter::convert(const std::string &literal)
{
	bool found = false;
	if(literal.size() == 1)
	{
		if(!(std::isdigit(static_cast<unsigned char>(literal[0]))))
		{
			found = true;
			if(std::isprint(static_cast<unsigned char>(literal[0])))
				std::cout << "char: '" << literal[0] << "'" << std::endl;
			if(!std::isprint(static_cast<unsigned char>(literal[0])))
				std::cout << "char: Non displayable" << std::endl;		
			int i = static_cast<int>(literal[0]);
			float f = static_cast<float>(literal[0]);
			double d = static_cast<double>(literal[0]);


			std::cout << "int: " << i << std::endl;

			if(f == static_cast<int>(f))
				std::cout << "float: " << f << ".0f" << std::endl;
			else
				std::cout << "float: " << f << "f" << std::endl;
			if (d == static_cast<int>(d))
				std::cout << "double: " << d << ".0" << std::endl;
			else
				std::cout << "double: " << d << std::endl;

		}
	}
	if (isIntLiteral(literal))
	{
		found = true;
		double value = std::strtod(literal.c_str(), NULL);

		if (value < CHAR_MIN || value > CHAR_MAX)
			std::cout << "char: impossible" << std::endl;
		else if (std::isprint(static_cast<unsigned char>(static_cast<int>(value))))
			std::cout << "char: '" << static_cast<char>(value) << "'" << std::endl;
		else
			std::cout << "char: Non displayable" << std::endl;

		if (value < INT_MIN || value > INT_MAX)
			std::cout << "int: impossible" << std::endl;
		else
			std::cout << "int: " << static_cast<int>(value) << std::endl;

		float f = static_cast<float>(value);
		if (f == static_cast<long>(f))
			std::cout << "float: " << f << ".0f" << std::endl;
		else
			std::cout << "float: " << f << "f" << std::endl;

		if (value == static_cast<long>(value))
			std::cout << "double: " << value << ".0" << std::endl;
		else
			std::cout << "double: " << value << std::endl;
	}
	if (isFloatLiteral(literal))
	{	
		found = true;
		float f = std::strtof(literal.c_str(), NULL);
		double d = static_cast<double>(f);
		bool isSpecial = std::isnan(f) || std::isinf(f);

		if (isSpecial || f < CHAR_MIN || f > CHAR_MAX)
			std::cout << "char: impossible" << std::endl;
		else if (std::isprint(static_cast<unsigned char>(static_cast<int>(f))))
			std::cout << "char: '" << static_cast<char>(f) << "'" << std::endl;
		else
			std::cout << "char: Non displayable" << std::endl;

		if (isSpecial || d < INT_MIN || d > INT_MAX)
			std::cout << "int: impossible" << std::endl;
		else
			std::cout << "int: " << static_cast<int>(f) << std::endl;

		if (isSpecial)
			std::cout << "float: " << f << "f" << std::endl;
		else if (f == static_cast<int>(f))
			std::cout << "float: " << f << ".0f" << std::endl;
		else
			std::cout << "float: " << f << "f" << std::endl;

		if (isSpecial)
			std::cout << "double: " << d << std::endl;
		else if (d == static_cast<int>(d))
			std::cout << "double: " << d << ".0" << std::endl;
		else
			std::cout << "double: " << d << std::endl;
	}
	if (isDoubleLiteral(literal))
	{
		found = true;
		double d = std::strtod(literal.c_str(), NULL);
		float f = static_cast<float>(d);
		bool isSpecial = std::isnan(d) || std::isinf(d);

		if (isSpecial || d < CHAR_MIN || d > CHAR_MAX)
			std::cout << "char: impossible" << std::endl;
		else if (std::isprint(static_cast<unsigned char>(static_cast<int>(d))))
			std::cout << "char: '" << static_cast<char>(d) << "'" << std::endl;
		else
			std::cout << "char: Non displayable" << std::endl;

		if (isSpecial || d < INT_MIN || d > INT_MAX)
			std::cout << "int: impossible" << std::endl;
		else
			std::cout << "int: " << static_cast<int>(d) << std::endl;

		if (isSpecial)
			std::cout << "float: " << f << "f" << std::endl;
		else if (f == static_cast<int>(f))
			std::cout << "float: " << f << ".0f" << std::endl;
		else
			std::cout << "float: " << f << "f" << std::endl;

		if (isSpecial)
			std::cout << "double: " << d << std::endl;
		else if (d == static_cast<int>(d))
			std::cout << "double: " << d << ".0" << std::endl;
		else
			std::cout << "double: " << d << std::endl;
	}
	if (!found)
	{
		std::cout << "char: impossible" << std::endl;
		std::cout << "int: impossible" << std::endl;
		std::cout << "float: impossible" << std::endl;
		std::cout << "double: impossible" << std::endl;
	}
}