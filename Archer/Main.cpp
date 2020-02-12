#include "DllInjector.h"
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		std::cout << "[*] Fatal Error. Usage: " << argv[0] << " [dll] [process]" << std::endl;
		return -1;
	}

	std::cout << argv[1] << std::endl;

	DllInjector dllInjector{ argv[1] };
	dllInjector.Inject(argv[2]);

	return 0;
}