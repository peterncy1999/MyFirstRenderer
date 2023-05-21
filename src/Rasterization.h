#pragma once
#include<algorithm>
#include"Window.h"
#include"Vector.h"
#include"Texture.h"
#include"Matrix.h"
void line(int x0, int y0, int x1, int y1, HDC hdc, int r, int g, int b);
void triangle_linesweep(Vec2i t0, Vec2i t1, Vec2i t2, HDC hdc, int r, int g, int b);
Vec3f getBarCor(Vec3f* vertex, Vec2i P);
void triangle_barycentric(Vec3f* vertex, float* zbuffer, int height, int width, HDC hdc, int* r, int* g, int* b);
void triangle_barycentric(Vec3f* vertex, float* zbuffer, int height, int width, HDC hdc, TGA** tex, Vec3f* texcord, Vec3f z_afterview, Vec3f* identity);
void triangle_barycentric_norm(Vec3f* vertex, float* zbuffer, int height, int width, HDC hdc, TGA** tex, Vec3f* texcord, Vec3f z_afterview, Vec3f lightPos, Matrix* trans, Vec3f* world_coords_afterview);
void triangle_barycentric_diffuse(Vec3f* vertex, float* zbuffer, int height, int width, HDC hdc, TGA* tex, Vec3f* texcord, Vec3f z_afterview, Vec3f* world_coords_afterview, Vec3f identity);
void rasterization_norm_shadow(Vec3f* vertex, float* zbuffer, int height, int width, HDC hdc, TGA** tex, Vec3f* texcord, Vec3f z_afterview, Vec3f lightPos, Matrix* trans, Vec3f* world_coords_afterview, float* shadowMap);