#include "DllInjector.h"
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		std::cout << "[*] Usage: " << argv[0] << " [dll] [process]" << std::endl;
		return -1;
	}

	try {
		DllInjector dllInjector{ argv[1] };
		dllInjector.Inject(argv[2]);

		std::cout << "[*] DLL Injection was completed successfully." << std::endl;
	} catch(std::exception &e) {
		std::cout << "[*] Fatal Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}