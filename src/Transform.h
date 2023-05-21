#pragma once
#include "Matrix.h"

Matrix projection(float znear, float zfar, float fov, float ratio);
Matrix view(Vec3f eyePos, Vec3f lookat, Vec3f up);
void projectiontest(float znear, float zfar, float fov, float ratio, Vec4f test);