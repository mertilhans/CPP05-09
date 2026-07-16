#include "whatever.hpp"

int main(void)
{
	int x = 1337;
	int y = 42;
	::swap(x,y);
	std::cout << "x =   " << x << "         ||||||||  y  =" << y << std::endl;

	double k = 13.37;
	double l = 42.42;
	::swap(k,l);
	std::cout << "a =   " << k << "    |||||||  y = " << l << std::endl;
	::max(k,l);
	std::cout << "min ==   " << ::min(k,l) << "max ==  " << ::max(k,l) << std::endl;

	float f = 13.371337;
	float fl = 42.4242;
	::swap(f,fl);
	std::cout << "f =   " << f << "   |||||||  fl = " <<fl << std::endl;

	int a = 2;
	int b = 3;
	::swap( a, b );
	std::cout << "a = " << a << ", b = " << b << std::endl;
	std::cout << "min( a, b ) = " << ::min( a, b ) << std::endl;
	std::cout << "max( a, b ) = " << ::max( a, b ) << std::endl;
	std::string c = "chaine1";
	std::string d = "chaine2";
	::swap(c, d);
	std::cout << "c = " << c << ", d = " << d << std::endl;
	std::cout << "min( c, d ) = " << ::min( c, d ) << std::endl;
	std::cout << "max( c, d ) = " << ::max( c, d ) << std::endl;
    return 0;
}
