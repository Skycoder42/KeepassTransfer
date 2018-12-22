#include "kptservice.h"
#include <iostream>

int main(int argc, char *argv[])
{
	// check if version
	for(auto i = 0; i < argc; i++) {
		if(qstrcmp(argv[i], "--version") == 0) {
			std::cout << VERSION << std::endl;
			return EXIT_SUCCESS;
		}
	}

	KPTService svc{argc, argv};
	return svc.exec();
}
