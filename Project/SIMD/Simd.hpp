#ifndef __SIMD_HPP__
#define __SIMD_HPP__
#include <xmmintrin.h>
#include <cstdint>
class SIMD {
	
};

void TestAddSSE()
{
	alignas(16) float A[4];
	alignas(16) float B[4] = { 2.0f, 4.0f, 6.0f, 8.0f };

	//set a (1,2,3,4) from literal
	//load b = (2,4,6,8) from an array
	//_mm_set_ps is backwards

	__m128 a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
	__m128 b = _mm_load_ps(&B[0]);

	__m128 result = _mm_add_ps(a, b);
	_mm_store_ps(&A[0], a);
	//storage for printing
	alignas(16) float R[4];
	_mm_store_ps(&R[0], result);

	printf("a = %.1f %.1f %.1f %.1f\n", A[0], A[1], A[2], A[3]);
	printf("b = %.1f %.1f %.1f %.1f\n", B[0], B[1], B[2], B[3]);
	printf("r = %.1f %.1f %.1f %.1f\n", R[0], R[1], R[2], R[3]);
}

#endif