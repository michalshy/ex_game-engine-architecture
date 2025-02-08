#include <iostream>
#include <cstdint>
#include "Errors.h"
#include "TypePunning.hpp"


int main()
{
	STATIC_ASSERT(sizeof(int) == 4);
	std::int32_t x = 128;
	printf("0x%08x\n", x);
	x = swapF32(x);
	printf("0x%08x\n", x);

}