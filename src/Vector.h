#pragma once
#include<cmath>
#include<iostream>
template<class T>
class Vec3 {
public:
	T x, y, z;
public:
	Vec3(): x(0), y(0), z(0) {}
	Vec3(T _x, T _y) : x(_x), y(_y), z(0) {}
	Vec3(T _x,T _y,T _z) : x(_x), y(_y), z(_z) {}
	Vec3 operator +(const Vec3<T>& v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
	Vec3 operator -(const Vec3<T>& v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
	Vec3 operator *(float f) const { return Vec3<T>(x * f, y * f, z * f); }
	Vec3 operator /(float f) const { return Vec3<T>(x / f, y / f, z / f); }
	Vec3<T> operator ^(const Vec3<T>& v) const { return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	T operator *(const Vec3<T>& v) const { return x * v.x + y * v.y + z * v.z; }
	Vec3& norm(T l = 1) {
		*this = (*this) * (l / std::sqrt(x * x + y * y + z * z));
		return *this;
	}
	friend std::ostream& operator << (std::ostream& s, Vec3<T>& v) {
		s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
		return s;
	}


};

typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

template<class T>
class Vec2 {
public:
	T x, y, z;
public:
	Vec2() : x(0), y(0) {}
	Vec2(T _x, T _y) : x(_x), y(_y){}
	Vec2 operator +(const Vec2<T>& v) const { return Vec2<T>(x + v.x, y + v.y); }
	Vec2 operator -(const Vec2<T>& v) const { return Vec2<T>(x - v.x, y - v.y); }
	Vec2 operator *(float f) const { return Vec2<T>(x * f, y * f); }
	Vec2 operator /(float f) const { return Vec2<T>(x / f, y / f); }
	T operator *(const Vec2<T>& v) const { return x * v.x + y * v.y ; }
	Vec3<T> operator ^(const Vec2<T>& v) const {
		//return Vec3<T>result(0, 0, x * v.y - y * v.x); 
		Vec3<T>result = Vec3<T>(0, 0, x * v.y - y * v.x);
		return result;
	}
	Vec2& norm(T l = 1) {
		*this = (*this) * (l / std::sqrt(x * x + y * y));
		return *this;
	}
	friend std::ostream& operator << (std::ostream& s, Vec2<T>& v) {
		s << "(" << v.x << ", " << v.y << ")\n";
		return s;
	}

	
};
typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;

template<class T>
class Vec4 {
public:
	T x, y, z, t;
public:
	Vec4() : x(0), y(0), z(0), t(0){}
	Vec4(T _x, T _y, T _z, T _t) : x(_x), y(_y), z(_z), t(_t) {}
	Vec4(Vec3<T> v, T _t) : x(v.x), y(v.y), z(v.z), t(_t) {}
	Vec4 operator +(const Vec4<T>& v) const { return Vec4<T>(x + v.x, y + v.y, z + v.z, t + v.t); }
	Vec4 operator -(const Vec4<T>& v) const { return Vec4<T>(x - v.x, y - v.y, z - v.z, t - v.t); }
	Vec4 operator *(float f) const { return Vec4<T>(x * f, y * f, z * f, t * f); }
	Vec4 operator /(float f) const { return Vec4<T>(x / f, y / f, z / f, t / f); }
	T operator *(const Vec4<T>& v) const { return x * v.x + y * v.y + z * v.z; }
	Vec4& norm(T l = 1) {
		*this = (*this) * (l / std::sqrt(x * x + y * y + z * z + t * t));
		return *this;
	}
	friend std::ostream& operator << (std::ostream& s, Vec4<T>& v) {
		s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.t  << ")\n";
		return s;
	}
	Vec4& standard() {
		*this = (*this) / (t);
		return *this;
	}

};

typedef Vec4<float> Vec4f;
typedef Vec4<int>   Vec4i;