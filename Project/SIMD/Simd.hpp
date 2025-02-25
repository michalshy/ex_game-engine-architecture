#ifndef __SIMD_HPP__
#define __SIMD_HPP__
#include <xmmintrin.h>
#include <cstdint>

#define _MM_TRANSPOSE4_PS(row0, row1, row2, row3) \
{ __m128 tmp3, tmp2, tmp1, tmp0; \
\
tmp0 = _mm_shuffle_ps((row0), (row1), 0x44); \
tmp2 = _mm_shuffle_ps((row0), (row1), 0xEE); \
tmp1 = _mm_shuffle_ps((row2), (row3), 0x44); \
tmp3 = _mm_shuffle_ps((row2), (row3), 0xEE); \
\
(row0) = _mm_shuffle_ps(tmp0, tmp1, 0x88); \
(row1) = _mm_shuffle_ps(tmp0, tmp1, 0xDD); \
(row2) = _mm_shuffle_ps(tmp2, tmp3, 0x88); \
(row3) = _mm_shuffle_ps(tmp2, tmp3, 0xDD); }


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

void SSELoop(int count,
	float* results,
	const float* dataA,
	const float* dataB)
{
	//size of 3 arrays have to be equal and multiple of 4
	assert(count % 4 == 0);
	for (int i = 0; i < count; i+=4)
	{
		__m128 a = _mm_load_ps(&dataA[i]);
		__m128 b = _mm_load_ps(&dataB[i]);
		__m128 result = _mm_add_ps(a, b);
		_mm_store_ps(&results[i], result);
	}
}

void NoSSELoop(int count,
	float* results,
	const float* dataA,
	const float* dataB)
{
	for (int i = 0; i < count; i++)
	{
		results[i] = dataA[i] + dataB[i];
	}
}

/*
* Given two arrays of four elements vectors, calculate their dot product
*/
void DotArrays_ref(int count,
	float r[],
	const float a[],
	const float b[])
{
	for (int i = 0; i < count; ++i)
	{
		const int j = i * 4;
		r[i] = a[j + 0] * b[j + 0] 
			+ a[j + 1] * b[j + 1] 
			+ a[j + 2] * b[j + 2] 
			+ a[j + 3] * b[j + 3];
	}
}

void DotArrays_sse(int count,
	float r[],
	const float a[],
	const float b[])
{
	for (int i = 0; i < count; i += 4)
	{
		__m128 vaX = _mm_load_ps(&a[(i + 0) * 4]);	// a[0], a[4], a[8], a[12]
		__m128 vaY = _mm_load_ps(&a[(i + 1) * 4]);	// a[1], a[5], a[9], a[13]
		__m128 vaZ = _mm_load_ps(&a[(i + 2) * 4]);	// a[2], a[6], a[10], a[14]
		__m128 vaW = _mm_load_ps(&a[(i + 3) * 4]);	// a[3], a[7], a[11], a[15]

		__m128 vbX = _mm_load_ps(&b[(i + 0) * 4]);	// b[0], b[4], b[8], b[12]
		__m128 vbY = _mm_load_ps(&b[(i + 1) * 4]);	// b[1], b[5], b[9], b[13]
		__m128 vbZ = _mm_load_ps(&b[(i + 2) * 4]);	// b[2], b[6], b[10], b[14]
		__m128 vbW = _mm_load_ps(&b[(i + 3) * 4]);	// b[3], b[7], b[11], b[15]

		__m128 result;
		result = _mm_mul_ps(vaX, vbX);
		result = _mm_add_ps(result, _mm_mul_ps(vaY, vbY));
		result = _mm_add_ps(result, _mm_mul_ps(vaZ, vbZ));
		result = _mm_add_ps(result, _mm_mul_ps(vaW, vbW));

		_mm_store_ps(&r[i], result);
	}
}

//SLOWER BECAUSE COMPILER OPTIMIZES REALLY WELL?
void DotArrays_sse_transpose(int count,
	float r[],
	const float a[],
	const float b[])
{
	for (int i = 0; i < count; i += 4)
	{
		__m128 vaX = _mm_load_ps(&a[(i + 0) * 4]);	// a[0], a[1], a[2], a[3]
		__m128 vaY = _mm_load_ps(&a[(i + 1) * 4]);	// a[4], a[5], a[6], a[7]
		__m128 vaZ = _mm_load_ps(&a[(i + 2) * 4]);	// a[8], a[9], a[10], a[11]
		__m128 vaW = _mm_load_ps(&a[(i + 3) * 4]);	// a[12], a[13], a[14], a[15]
		__m128 vbX = _mm_load_ps(&b[(i + 0) * 4]);	// b[0], b[1], b[2], b[3]
		__m128 vbY = _mm_load_ps(&b[(i + 1) * 4]);	// b[4], b[5], b[6], b[7]
		__m128 vbZ = _mm_load_ps(&b[(i + 2) * 4]);	// b[8], b[9], b[10], b[11]
		__m128 vbW = _mm_load_ps(&b[(i + 3) * 4]);	// b[12], b[13], b[14], b[15]
		_MM_TRANSPOSE4_PS(vaX, vaY, vaZ, vaW);
		_MM_TRANSPOSE4_PS(vbX, vbY, vbZ, vbW);
		__m128 result;
		result = _mm_mul_ps(vaX, vbX);
		result = _mm_add_ps(result, _mm_mul_ps(vaY, vbY));
		result = _mm_add_ps(result, _mm_mul_ps(vaZ, vbZ));
		result = _mm_add_ps(result, _mm_mul_ps(vaW, vbW));
		_mm_store_ps(&r[i], result);
	}
}

#endif