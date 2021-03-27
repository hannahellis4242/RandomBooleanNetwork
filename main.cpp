#include <iostream>
#include "Network.h"

int main()
{
	std::cout << rbn::run(rbn::Network(50,5,1),250);
	return 0;
}

