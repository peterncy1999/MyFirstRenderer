#include "Transform.h"

Matrix projection(float znear, float zfar, float fov, float ratio) {
	Matrix project;
	project.val[0][0] = znear;
	project.val[1][1] = znear;
	project.val[2][2] = znear + zfar;
	project.val[2][3] = -znear * zfar;
	project.val[3][2] = 1;
	float t = znear * tan((fov / 2 * acos(-1) / 180));
	float b = -t;
	float l = -t * ratio;
	float r = -l;

	Matrix orth1;
	orth1.val[0][0] = 1;
	orth1.val[0][3] = -(r + l) / 2;
	orth1.val[1][1] = 1;
	orth1.val[1][3] = -(t + b) / 2;
	orth1.val[2][2] = 1;
	orth1.val[2][3] = -(zfar + znear) / 2;
	orth1.val[3][3] = 1;

	Matrix orth2;
	orth2.val[0][0] = 2 / (r - l);
	orth2.val[1][1] = 2 / (t - b);
	orth2.val[2][2] = 2 / (znear - zfar);
	orth2.val[3][3] = 1;

	Matrix result;
	result = orth2 * orth1 *  project;
	return result;
}

Matrix view(Vec3f eyePos, Vec3f lookat, Vec3f up) {
	Vec3f right = lookat ^ up;
	right.norm();
	lookat.norm();
	up.norm();
	Matrix move;
	move.val[0][0] = 1;
	move.val[0][3] = -eyePos.x;
	move.val[1][1] = 1;
	move.val[1][3] = -eyePos.y;
	move.val[2][2] = 1;
	move.val[2][3] = -eyePos.z;
	move.val[3][3] = 1;
	Matrix rotate;
	rotate.val[0][0] = right.x;
	rotate.val[0][1] = right.y;
	rotate.val[0][2] = right.z;
	rotate.val[1][0] = up.x;
	rotate.val[1][1] = up.y;
	rotate.val[1][2] = up.z;
	rotate.val[2][0] = -lookat.x;
	rotate.val[2][1] = -lookat.y;
	rotate.val[2][2] = -lookat.z;
	rotate.val[3][3] = 1;
	return rotate * move;

}

void projectiontest(float znear, float zfar, float fov, float ratio, Vec4f test) {
	Matrix project;
	project.val[0][0] = znear;
	project.val[1][1] = znear;
	project.val[2][2] = znear + zfar;
	project.val[2][3] = -znear * zfar;
	project.val[3][2] = 1;
	float t = znear * tan((fov / 2 * acos(-1) / 180));
	float b = -t;
	float l = -t * ratio;
	float r = -l;

	Matrix orth1;
	orth1.val[0][0] = 1;
	orth1.val[0][3] = -(r + l) / 2;
	orth1.val[1][1] = 1;
	orth1.val[1][3] = -(t + b) / 2;
	orth1.val[2][2] = 1;
	orth1.val[2][3] = -(zfar + znear) / 2;
	orth1.val[3][3] = 1;

	Matrix orth2;
	orth2.val[0][0] = 2 / (r - l);
	orth2.val[1][1] = 2 / (t - b);
	orth2.val[2][2] = 2 / (znear - zfar);
	orth2.val[3][3] = 1;

	Matrix result;
	project.show();
	test =  project * test;
	std :: cout << test;
	orth1.show();
	test = orth1 * test;
	std::cout << test;
	orth2.show();
	test = orth2 * test;
	std::cout << test;
}