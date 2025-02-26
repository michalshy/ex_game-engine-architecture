#ifndef __GPGPU_HPP__
#define __GPGPU_HPP__

void DotKernel(unsigned i,
	float r[],
	const float a[],
	const float b[])
{
	// treat each block of four floats as a
	// single four-element vector
	const unsigned j = i * 4;
	r[i] = a[j + 0] * b[j + 0] // ax*bx
		+ a[j + 1] * b[j + 1] // ay*by
		+ a[j + 2] * b[j + 2] // az*bz
		+ a[j + 3] * b[j + 3]; // aw*bw
}

void DotArrays_gpgpu1(unsigned count,
	float r[],
	const float a[],
	const float b[])
{
	for (unsigned i = 0; i < count; ++i)
	{
		DotKernel(i, r, a, b);
	}
}


#endif