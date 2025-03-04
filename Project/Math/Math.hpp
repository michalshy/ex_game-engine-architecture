#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

//Nick Bobic - 1998, Gamasutra
void MatrixToQuaternion(
	const float R[3][3],
	float q[4]
)
{
	float trace = R[0][0] + R[1][1] + R[2][2];

	//CHECK THE DIAGONAL
	if (trace > 0.0f)
	{
		float s = sqrt(trace + 1.0f);
		q[3] = s * 0.5f;

		float t = 0.5f;
		q[0] = (R[2][1] - R[1][2]) * t;
		q[1] = (R[0][2] - R[2][0]) * t;
		q[2] = (R[1][0] - R[0][1]) * t;
	}
	else
	{
		//negative diagonal
		int i = 0;
		if (R[1][1] > R[0][0]) i = 1;
		if (R[2][2] > R[i][i]) i = 2;

		static const int NEXT[3] = { 1,2,0 };

		int j = NEXT[i];
		int k = NEXT[j];

		float s = sqrt((R[i][j] - R[j][j] + R[k][k] + 1.0f));
		q[i] = s * 0.5f;
		float t;
		if (s != 0.0) t = 0.5f / s;
		else t = s;
		q[3] = (R[k][j] - R[j][k]) * t;
		q[j] = (R[j][i] - R[i][j]) * t;
		q[k] = (R[k][i] - R[i][k]) * t;
	}
}

#endif