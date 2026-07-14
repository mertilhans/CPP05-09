#ifndef SCALARCONVERTER_HPP
#define SCALARCONVERTER_HPP

#include <string>

class ScalarConverter
{
	private:
		ScalarConverter();
		static bool isIntLiteral(const std::string &literal);
		static bool isFloatLiteral(const std::string &literal);
		static bool isDoubleLiteral(const std::string &literal);

	public:
		static void convert(const std::string &literal);

};



#endif