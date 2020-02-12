#include "DllInjector.h"
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cout << "[*] Fatal Error. Usage: " << argv[0] << " [process]" << std::endl;
		return -1;
	}

	DllInjector dllInjector{ "C:\\Users\\Omer Katz\\source\\repos\\Archer\\x64\\Debug\\Hooker.dll" };
	dllInjector.Inject(argv[1]);

	return 0;
}