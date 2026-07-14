#include "Serializer.hpp"
#include <iostream>

int main()
{
	Data data;
	data.id = 1337;
	data.name = "leet";

	Data *original = &data;

	std::cout << "--- BEFORE ---" << std::endl;
	std::cout << "original address:  " << original << std::endl;
	std::cout << "original id:       " << original->id << std::endl;
	std::cout << "original name:     " << original->name << std::endl;

	uintptr_t raw = Serializer::serialize(original);
	Data *restored = Serializer::deserialize(raw);

	std::cout << "--- AFTER ---" << std::endl;
	std::cout << "serialized value:  " << std::hex << std::showbase << raw << std::dec << std::endl;
	std::cout << "restored address:  " << restored << std::endl;
	std::cout << "restored id:       " << restored->id << std::endl;
	std::cout << "restored name:     " << restored->name << std::endl;

	if (restored == original)
		std::cout << "OK: pointers are identical" << std::endl;
	else
		std::cout << "FAIL: pointers different" << std::endl;
	return 0;
}

