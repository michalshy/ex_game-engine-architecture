#include <iostream>
#include <cstdint>
#include <thread>
#include <Windows.h>
#include "Test/Errors.hpp"
#include "Test/TypePunning.hpp"
#include "Concurrency/SpinLock.hpp"


static int a = 10;
static CRITICAL_SECTION  critSec;

void task(int i)
{
	//shows how threads are interupting eachother
	std::cout << "my id: "<< i << " and a is: " << a << std::endl;
	EnterCriticalSection(&critSec);
	std::cout << i << " is incrementing" << std::endl;
	++a;
	std::cout << i << " finishing" << std::endl;
	LeaveCriticalSection(&critSec);
}

int main()
{
#pragma region threads
	{
		InitializeCriticalSection(&critSec);
		std::thread t[3];
		for (int i = 0; i < 3; i++)
		{
			t[i] = std::thread(task, i);
		}

		for (int i = 0; i < 3; i++)
		{
			t[i].join();
		}
		DeleteCriticalSection(&critSec);
	}
#pragma endregion

#pragma region asserts_swap
	STATIC_ASSERT(sizeof(int) == 4);
	std::int32_t x = 128;
	printf("0x%08x\n", x);
	x = swapF32(x);
	printf("0x%08x\n", x);
#pragma endregion


#pragma region concurrency
	TestLocks();

#pragma endregion

}