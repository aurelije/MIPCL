#include <iostream>
#include "timeindex.h"

int main(int argc, const char *argv[])
{
	try {
		Ctimeindex prob(argv[1]);
		prob.model();
	}
	catch(CException* pe) {
		std::cerr << pe->what() << std::endl;
		delete pe;
		return 1;
	}
	return 0;
}

