#include "Matrix.h"

Matrix::Matrix() {
	val = std::vector<std::vector<float>>(4, std::vector<float>(4, 0));
}
Matrix::Matrix(Vec4f a, Vec4f b, Vec4f c, Vec4f d){
	val = std::vector<std::vector<float>>(4, std::vector<float>(4, 0));
	val[0][0] = a.x; val[0][1] = a.y; val[0][2] = a.z; val[0][3] = a.t;
	val[1][0] = b.x; val[1][1] = b.y; val[1][2] = b.z; val[1][3] = b.t;
	val[2][0] = c.x; val[2][1] = c.y; val[2][2] = c.z; val[2][3] = c.t;
	val[3][0] = d.x; val[3][1] = d.y; val[3][2] = d.z; val[3][3] = d.t;
}
Matrix Matrix::identity() {
	Matrix I;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == j)I.val[i][j] = 1;
			else { I.val[i][j] = 0; }
		}
	}
	return I;
}
Matrix Matrix::operator+(const Matrix& a) {
	Matrix result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.val[i][j] = val[i][j] + a.val[i][j];
		}
	}
	return result;
}
Matrix Matrix::operator-(const Matrix& a) {
	Matrix result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.val[i][j] = val[i][j] - a.val[i][j];
		}
	}
	return result;
}
Matrix Matrix::operator/(float a) {
	Matrix result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.val[i][j] = val[i][j] / a;
		}
	}
	return result;
}
Matrix Matrix::operator*(float a) {
	Matrix result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.val[i][j] = val[i][j] * a;
		}
	}
	return result;
}
Matrix Matrix::operator*(const Matrix& a) {
	Matrix result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				result.val[i][j] += val[i][k] * a.val[k][j];
			}
		}
	}
	return result;
}

Vec4f Matrix::operator*(const Vec4f& a) {
	Vec4f result;
	result.x = a.x * val[0][0] + a.y * val[0][1] + a.z * val[0][2] + a.t * val[0][3];
	result.y = a.x * val[1][0] + a.y * val[1][1] + a.z * val[1][2] + a.t * val[1][3];
	result.z = a.x * val[2][0] + a.y * val[2][1] + a.z * val[2][2] + a.t * val[2][3];
	result.t = a.x * val[3][0] + a.y * val[3][1] + a.z * val[3][2] + a.t * val[3][3];
	return result;
}

Matrix Matrix::transpose() {
	Matrix result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {		 
			result.val[i][j] = val[j][i];			
		}
	}
	return result;
}
void Matrix::show() {
	std::cout << "{" << std::endl;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 3; ++j) {
			std::cout << val[i][j] << ", ";
		}
		std::cout << val[i][3] << std::endl;
	}
	std::cout << "}" << std::endl;
}

float Matrix::det() {
	float result = val[0][0] * (  val[1][1] * (val[2][2] * val[3][3] - val[3][2] * val[2][3]) 
							    - val[1][2] * (val[2][1] * val[3][3] - val[3][1] * val[2][3])
								+ val[1][3] * (val[2][1] * val[3][2] - val[3][1] * val[2][2]))
				 - val[0][1] * (  val[1][0] * (val[2][2] * val[3][3] - val[3][2] * val[2][3])
							    - val[1][2] * (val[2][0] * val[3][3] - val[3][0] * val[2][3])
			                    + val[1][3] * (val[2][0] * val[3][2] - val[3][0] * val[2][2]))
				 + val[0][2] * (  val[1][0] * (val[2][1] * val[3][3] - val[3][1] * val[2][3])
								- val[1][1] * (val[2][0] * val[3][3] - val[3][0] * val[2][3])
								+ val[1][3] * (val[2][0] * val[3][1] - val[3][0] * val[2][1]))
				 - val[0][3] * (  val[1][0] * (val[2][1] * val[3][2] - val[3][1] * val[2][2])
								- val[1][1] * (val[2][0] * val[3][2] - val[3][0] * val[2][2])
								+ val[1][2] * (val[2][0] * val[3][1] - val[3][0] * val[2][1]));	
	return result;
}

Matrix Matrix::inverse() { //ref: https://blog.csdn.net/baidu_41922078/article/details/124351331
	Matrix t = *this;

	Matrix b;
	b = b.identity();
	float max, temp;
	for (int i = 0; i < 4; ++i) {
		max = t.val[i][i];
		// 用于记录每一列中的第几个元素为主元
		int k = i;
		// 寻找每一列中的主元元素
		for (int j = i; j < 4; ++j) {
			if (fabs(t.val[j][i]) > fabs(max)) {
				max = t.val[j][i];
				k = j;
			}
		}
		// 如果主元所在的行不是第i行，则进行行交换
		if (k != i) {
			// 进行行交换
			for (int j = 0; j < 4; j++) {
				temp = t.val[i][j];
				t.val[i][j] = t.val[k][j];
				t.val[k][j] = temp;
				// 伴随矩阵B也要进行行交换
				temp = b.val[i][j];
				b.val[i][j] = b.val[k][j];
				b.val[k][j] = temp;
			}
		}
		if (t.val[i][i] == 0) {
			std::cout << "no inverse matrix" << std::endl;
			break;
		}
		temp = t.val[i][i];

		for (int j = 0; j < 4; j++)
		{
			t.val[i][j] = t.val[i][j] / temp;
			b.val[i][j] = b.val[i][j] / temp;
		}
		for (int j = 0; j < 4; j++)
		{
			if (j != i)
			{
				temp = t.val[j][i];
				//消去该列的其他元素
				for (int k = 0; k < 4; k++)
				{
					t.val[j][k] = t.val[j][k] - temp * t.val[i][k];
					b.val[j][k] = b.val[j][k] - temp * b.val[i][k];
				}
			}

		}
	}
	return b;
}
