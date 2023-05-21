#pragma once
#include "Vector.h"
#include <vector>
#include <math.h>
class Matrix
{
public:
	std::vector<std::vector<float>>val;
public:
	Matrix();
	Matrix(Vec4f a, Vec4f b, Vec4f c, Vec4f d);
	Matrix identity();
	Matrix operator+(const Matrix& a);
	Matrix operator-(const Matrix& a);
	Matrix operator*(const Matrix& a);
	Matrix operator/(float a);
	Matrix operator*(float a);
	Vec4f operator*(const Vec4f& a);
	Matrix transpose();
	void show();
	float det();
	Matrix inverse();
};