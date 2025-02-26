
#include <iostream>
#include <cstdint>
#include <thread>
#include <Windows.h>
#include "Test/Errors.hpp"
#include "Test/TypePunning.hpp"
#include "Concurrency/SpinLock.hpp"
#include "Concurrency/List.hpp"
#include "SIMD/Simd.hpp"
#include <chrono>
#include "GPGPU/GPGPU.hpp"

#undef max()
#include <limits>
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
	TestUL();

#pragma endregion
#pragma region SIMD
	TestAddSSE();
	//Test vectorized loop
	//int count = 40240 * 40240;
	int count = 5192 * 5192;
	float* results = new float[count];
	float* dataA = new float[count];
	float* dataB = new float[count];
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	NoSSELoop(count, results, dataA, dataB);
	std::cout << "Time difference = " 
		<< std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() 
		<< "[탎]" 
		<< std::endl;
	begin = std::chrono::steady_clock::now();
	SSELoop(count, results, dataA, dataB);
	std::cout << "Time difference = " 
		<< std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::steady_clock::now() - begin).count()
		<< "[탎]" 
		<< std::endl;
	//int c = 10240 * 10240;
	int c = 2096 * 2096;
	begin = std::chrono::steady_clock::now();
	DotArrays_sse_transpose(c, results, dataA, dataB);
	std::cout << "Time difference = "
		<< std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::steady_clock::now() - begin).count()
		<< "[탎]"
		<< std::endl;
	begin = std::chrono::steady_clock::now();
	DotArrays_ref(c, results, dataA, dataB);
	std::cout << "Time difference = "
		<< std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::steady_clock::now() - begin).count()
		<< "[탎]"
		<< std::endl;
	begin = std::chrono::steady_clock::now();
	DotArrays_sse(c, results, dataA, dataB);
	std::cout << "Time difference = "
		<< std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::steady_clock::now() - begin).count()
		<< "[탎]"
		<< std::endl;

#pragma endregion
#pragma region GPGPU



#pragma endregion

}