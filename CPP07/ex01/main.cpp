#include "iter.hpp"

int main(void)
{
	int arr[4] = {1,3,3,7};
	std::cout << "original array " << std::endl;
	::iter(arr,4,print<int>);
	std::cout << "----------------------" << std::endl;

	::iter(arr,4,increment<int>);
	::iter(arr,4,print<int>);
		std::cout << "----------------------" << std::endl;

	int *nullpt = NULL;
	::iter(nullpt,5,print<int>);

	const char *str[] = {"say", "hello" , "to", "my" , "little", "friends"};
	::iter(str,6,print<const char*>);



    return 0;
}
