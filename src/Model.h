#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Vector.h"
class Model {
public:
	std::vector<Vec3f> v;
	std::vector<Vec3f> vt;
	std::vector<Vec3f> vn;
	std::vector<std::vector<int>> f;
public:
	Model(const char* filename);
	~Model();
};