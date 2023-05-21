#pragma once
#include"Model.h"
#include"Rasterization.h"
#include <limits>
#include <cstring>
#include "Window.h"
#include "Texture.h"
#include "Matrix.h"

class Render {
public:
	Model* obj;

public:
	void renderobj(const char* filename,  std::vector<std::string> texname, int height, int width, float* zbuffer,Matrix* trans ,HDC hdc);
	void renderobj_norm(const char* filename, std::vector<std::string> texname, int height, int width, float* zbuffer, Matrix* trans, HDC hdc);
	void renderobj_diffuse(const char* filename, std::vector<std::string> texname, int height, int width, float* zbuffer, Matrix* trans, HDC hdc);
	void get_shadowmap(const char* filename, int height, int width, float* zbuffer, Matrix* trans, Vec3f light_pos, HDC hdc);
	void renderwithshadow_norm(const char* filename, std::vector<std::string> texname, int height, int width, float* zbuffer, float* shadowMap, Vec3f light_pos, Matrix* trans, HDC hdc);
	void renderwithshadow_diffuse(const char* filename, std::vector<std::string> texname, int height, int width, float* zbuffer, float* shadowMap, Vec3f light_pos, Matrix* trans, HDC hdc);
};