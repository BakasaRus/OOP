#include "primeFactors.h"
#include <iostream>

int main()
{
	PrimeFactors PF(57455309245);
	std::cout << PF.ToString() << std::endl;
	system("pause");
	return 0;
}