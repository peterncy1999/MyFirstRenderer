#include"Rasterization.h"
//Bresenham直线算法
void line(int x0, int y0, int x1, int y1, HDC hdc, int r, int g, int b) {
	bool steep = false;
	bool xreverse = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	if (y1 < y0) {
		y0 = -y0;
		y1 = -y1;
		xreverse = true;
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int p = 2 * dy - dx;
	int y = y0;

	if (steep && xreverse) {
		SetPixel(hdc, -y0, x0, RGB(r, g, b));
		SetPixel(hdc, -y1, x1, RGB(r, g, b));
	}
	else if (steep && !xreverse) {
		SetPixel(hdc, y0, x0, RGB(r, g, b));
		SetPixel(hdc, y1, x1, RGB(r, g, b));
	}
	else if (!steep && xreverse) {
		SetPixel(hdc, x0, -y0, RGB(r, g, b));
		SetPixel(hdc, x1, -y1, RGB(r, g, b));
	}
	else if (!steep && !xreverse) {
		SetPixel(hdc, x0, y0, RGB(r, g, b));
		SetPixel(hdc, x1, y1, RGB(r, g, b));
	}
	for (int x = x0; x < x1 - 1; ++x) {

		if (p <= 0) {
			if (steep && xreverse) {
				SetPixel(hdc, -y, x + 1, RGB(r, g, b));
			}
			else if (steep && !xreverse) {
				SetPixel(hdc, y, x + 1, RGB(r, g, b));
			}
			else if (!steep && xreverse) {
				SetPixel(hdc, x + 1, -y, RGB(r, g, b));
			}
			else if (!steep && !xreverse) {
				SetPixel(hdc, x + 1, y, RGB(r, g, b));
			}
			p += 2 * dy;
		}
		else {
			y++;
			if (steep && xreverse) {
				SetPixel(hdc, -y, x + 1, RGB(r, g, b));
			}
			else if (steep && !xreverse) {
				SetPixel(hdc, y, x + 1, RGB(r, g, b));
			}
			else if (!steep && xreverse) {
				SetPixel(hdc, x + 1, -y, RGB(r, g, b));
			}
			else if (!steep && !xreverse) {
				SetPixel(hdc, x + 1, y, RGB(r, g, b));
			}
			p = p + 2 * dy - 2 * dx;
		}
	}

}
//扫描线
void triangle_linesweep(Vec2i t0, Vec2i t1, Vec2i t2, HDC hdc, int r, int g, int b) {
	if (t0.y == t1.y && t0.y == t2.y) {
		line(t0.x, t0.y, t1.x, t1.y, hdc, r, g, b);
		line(t2.x, t2.y, t1.x, t1.y, hdc, r, g, b);
	}
	if (t0.x == t1.x && t0.x == t2.x) {
		line(t0.x, t0.y, t1.x, t1.y, hdc, r, g, b);
		line(t2.x, t2.y, t1.x, t1.y, hdc, r, g, b);
	}
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);
	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; ++i) {
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
		Vec2i A = t0 + (t2 - t0) * alpha;
		Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		line(A.x, t0.y + i, B.x, t0.y + i, hdc, r, g, b);
	}
}
//重心
Vec3f getBarCor(Vec3f* vertex, Vec2i P) {
	Vec3f area = Vec3f(vertex[1].x - vertex[0].x, vertex[2].x - vertex[0].x, vertex[0].x - P.x) ^ Vec3f(vertex[1].y - vertex[0].y, vertex[2].y - vertex[0].y, vertex[0].y - P.y);
	if (abs(area.z) < 1) return Vec3f(-1, 1, 1);
	return Vec3f(1.f - (area.x + area.y) / area.z, area.x / area.z, area.y / area.z); //(1-u-v),u,v
}

void triangle_barycentric(Vec3f* vertex, float* zbuffer, int height, int width, HDC hdc, int* r, int* g, int* b) {
	Vec2i boxmin( (min(min(vertex[0].x, vertex[1].x), vertex[2].x)), (min(min(vertex[0].y, vertex[1].y), vertex[2].y)));
	Vec2i boxmax(( max(max(vertex[0].x, vertex[1].x), vertex[2].x)), (max(max(vertex[0].y, vertex[1].y), vertex[2].y)));
	Vec3f P;
	P.z = 0;
	for (P.x = boxmin.x; P.x <= boxmax.x; P.x++) {
		for (P.y = boxmin.y; P.y <= boxmax.y; P.y++) {
			Vec3f baryPara = getBarCor(vertex, Vec2i(P.x,P.y));
			if (baryPara.x < 0 || baryPara.y < 0 || baryPara.z < 0) continue;
			P.z = 0;
			P.z = vertex[0].z * baryPara.x + vertex[1].z * baryPara.y + vertex[2].z * baryPara.z;
			int red= r[0] * baryPara.x + r[1] * baryPara.y + r[2] * baryPara.z;
			int green = g[0] * baryPara.x + g[1] * baryPara.y + g[2] * baryPara.z;
			int blue = b[0] * baryPara.x + b[1] * baryPara.y + b[2] * baryPara.z;

			if (zbuffer[int(P.x + P.y * width)] < P.z) {
				zbuffer[int(P.x + P.y * width)] = P.z;
				//std::cout << zbuffer[int(P.x + P.y * height)] << std::endl;
				SetPixel(hdc, P.x, P.y, RGB(red, green, blue));
			}
		}
	}
}

void triangle_barycentric_diffuse(Vec3f* vertex, float* zbuffer, int height, int width, HDC hdc, TGA* tex, Vec3f* texcord, Vec3f z_afterview, Vec3f* world_coords_afterview, Vec3f identity) {
	Vec2i boxmin((min(min(vertex[0].x, vertex[1].x), vertex[2].x)), (min(min(vertex[0].y, vertex[1].y), vertex[2].y)));
	Vec2i boxmax((max(max(vertex[0].x, vertex[1].x), vertex[2].x)), (max(max(vertex[0].y, vertex[1].y), vertex[2].y)));
	Vec3f P;
	P.z = 0;
	for (P.x = boxmin.x; P.x <= boxmax.x; P.x++) {
		for (P.y = boxmin.y; P.y <= boxmax.y; P.y++) {
			Vec3f baryPara = getBarCor(vertex, Vec2i(P.x, P.y));
			if (baryPara.x < 0 || baryPara.y < 0 || baryPara.z < 0) continue;
			P.z = 0;
			P.z = vertex[0].z * baryPara.x + vertex[1].z * baryPara.y + vertex[2].z * baryPara.z;
			Vec3f tex_insertcord;
			Vec3f insert_para;
			insert_para.x = baryPara.x / z_afterview.x;
			insert_para.y = baryPara.y / z_afterview.y;
			insert_para.z = baryPara.z / z_afterview.z;
			tex_insertcord.x = texcord[0].x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[1].x * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[2].x * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			tex_insertcord.y = texcord[0].y * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[1].y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[2].y * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);

			float light_diffuse = identity.x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ identity.y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ identity.z * insert_para.z / (insert_para.x + insert_para.y + insert_para.z) ;
			//std::cout << light << std::endl;
			int b = tex->data[((int)(tex_insertcord.x * (tex->width - 1)) + (int)(tex_insertcord.y * (tex->height - 1)) * tex->width) * tex->bytespp] * light_diffuse;
			int g = tex->data[((int)(tex_insertcord.x * (tex->width - 1)) + (int)(tex_insertcord.y * (tex->height - 1)) * tex->width) * tex->bytespp + 1] * light_diffuse;
			int r = tex->data[((int)(tex_insertcord.x * (tex->width - 1)) + (int)(tex_insertcord.y * (tex->height - 1)) * tex->width) * tex->bytespp + 2] * light_diffuse;

			Vec3f cor_insertcord;
			cor_insertcord.x = world_coords_afterview[0].x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[1].x * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[2].x * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			cor_insertcord.y = world_coords_afterview[0].y * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[1].y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[2].y * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			cor_insertcord.z = world_coords_afterview[0].z * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[1].z * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[2].z * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			P.z = cor_insertcord.z;
			//std::cout << r << ", " << g << ", " << b << std::endl;

			//std::cout << r << ", " << g << ", " << b << std::endl;
			//std::cout <<"-----------" << std::endl;


			if (zbuffer[int(P.x + P.y * width)] < P.z) {
				zbuffer[int(P.x + P.y * width)] = P.z;
				//std::cout << zbuffer[int(P.x + P.y * height)] << std::endl;
				SetPixel(hdc, P.x, P.y, RGB(min(r, 255), min(g, 255), min(b, 255)));
			}
		}
	}
}

void triangle_barycentric(Vec3f* vertex, float* zbuffer, int height, int width, HDC hdc, TGA** tex, Vec3f* texcord, Vec3f z_afterview, Vec3f* identity) {
	Vec2i boxmin((min(min(vertex[0].x, vertex[1].x), vertex[2].x)), (min(min(vertex[0].y, vertex[1].y), vertex[2].y)));
	Vec2i boxmax((max(max(vertex[0].x, vertex[1].x), vertex[2].x)), (max(max(vertex[0].y, vertex[1].y), vertex[2].y)));
	Vec3f P;
	P.z = 0;
	for (P.x = boxmin.x; P.x <= boxmax.x; P.x++) {
		for (P.y = boxmin.y; P.y <= boxmax.y; P.y++) {
			Vec3f baryPara = getBarCor(vertex, Vec2i(P.x, P.y));
			if (baryPara.x < 0 || baryPara.y < 0 || baryPara.z < 0) continue;
			P.z = 0;
			P.z = vertex[0].z * baryPara.x + vertex[1].z * baryPara.y + vertex[2].z * baryPara.z;
			Vec3f tex_insertcord;
			Vec3f insert_para;
			insert_para.x = baryPara.x / z_afterview.x;
			insert_para.y = baryPara.y / z_afterview.y;
			insert_para.z = baryPara.z / z_afterview.z;
			tex_insertcord.x = texcord[0].x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[1].x * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[2].x * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			tex_insertcord.y = texcord[0].y * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[1].y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[2].y * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);

			float light_diffuse = identity[0].x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ identity[0].y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ identity[0].z * insert_para.z / (insert_para.x + insert_para.y + insert_para.z) + 0.2;
			//std::cout << light << std::endl;
			int b = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp] * light_diffuse;
			int g = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp + 1] * light_diffuse;
			int r = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp + 2] * light_diffuse;

			//std::cout << r << ", " << g << ", " << b << std::endl;
			float light_spec = identity[1].x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ identity[1].y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ identity[1].z * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			
			b +=  tex[1]->data[((int)(tex_insertcord.x * (tex[1]->width - 1)) + (int)(tex_insertcord.y * (tex[1]->height - 1)) * tex[1]->width) * tex[1]->bytespp] * light_spec;
			g += tex[1]->data[((int)(tex_insertcord.x * (tex[1]->width - 1)) + (int)(tex_insertcord.y * (tex[1]->height - 1)) * tex[1]->width) * tex[1]->bytespp + 1] * light_spec;
			r += tex[1]->data[((int)(tex_insertcord.x * (tex[1]->width - 1)) + (int)(tex_insertcord.y * (tex[1]->height - 1)) * tex[1]->width) * tex[1]->bytespp + 2] * light_spec;
			//std::cout << r << ", " << g << ", " << b << std::endl;
			float light_glow = identity[2].x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ identity[2].y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ identity[2].z * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);

			b += tex[2]->data[((int)(tex_insertcord.x * (tex[2]->width - 1)) + (int)(tex_insertcord.y * (tex[2]->height - 1)) * tex[2]->width) * tex[2]->bytespp] * light_glow;
			g += tex[2]->data[((int)(tex_insertcord.x * (tex[2]->width - 1)) + (int)(tex_insertcord.y * (tex[2]->height - 1)) * tex[2]->width) * tex[2]->bytespp + 1] * light_glow;
			r += tex[2]->data[((int)(tex_insertcord.x * (tex[2]->width - 1)) + (int)(tex_insertcord.y * (tex[2]->height - 1)) * tex[2]->width) * tex[2]->bytespp + 2] * light_glow;
			//std::cout << r << ", " << g << ", " << b << std::endl;
			//std::cout <<"-----------" << std::endl;


			if (zbuffer[int(P.x + P.y * width)] < P.z) {
				zbuffer[int(P.x + P.y * width)] = P.z;
				//std::cout << zbuffer[int(P.x + P.y * height)] << std::endl;
				SetPixel(hdc, P.x, P.y, RGB(min(r, 255), min(g, 255), min(b, 255)));
			}
		}
	}
}

void triangle_barycentric_norm(Vec3f* vertex, float* zbuffer, int height, int width, HDC hdc, TGA** tex, Vec3f* texcord, Vec3f z_afterview, Vec3f lightPos, Matrix* trans, Vec3f* world_coords_afterview) {
	Vec2i boxmin((min(min(vertex[0].x, vertex[1].x), vertex[2].x)), (min(min(vertex[0].y, vertex[1].y), vertex[2].y)));
	Vec2i boxmax((max(max(vertex[0].x, vertex[1].x), vertex[2].x)), (max(max(vertex[0].y, vertex[1].y), vertex[2].y)));
	Vec3f P;
	P.z = 0;
	for (P.x = boxmin.x; P.x <= boxmax.x; P.x++) {
		for (P.y = boxmin.y; P.y <= boxmax.y; P.y++) {
			Vec3f baryPara = getBarCor(vertex, Vec2i(P.x, P.y));
			if (baryPara.x < 0 || baryPara.y < 0 || baryPara.z < 0) continue;
			P.z = 0;
			P.z = vertex[0].z * baryPara.x + vertex[1].z * baryPara.y + vertex[2].z * baryPara.z;
			Vec3f tex_insertcord;
			Vec3f insert_para;
			insert_para.x = baryPara.x / z_afterview.x;
			insert_para.y = baryPara.y / z_afterview.y;
			insert_para.z = baryPara.z / z_afterview.z;
			tex_insertcord.x = texcord[0].x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[1].x * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[2].x * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			tex_insertcord.y = texcord[0].y * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[1].y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[2].y * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			
			Vec3f cor_insertcord;
			cor_insertcord.x = world_coords_afterview[0].x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[1].x * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[2].x * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			cor_insertcord.y = world_coords_afterview[0].y * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[1].y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[2].y * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			cor_insertcord.z = world_coords_afterview[0].z * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[1].z * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[2].z * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			P.z = cor_insertcord.z;


			Vec4f norm;
			norm.x = tex[3]->data[((int)(tex_insertcord.x * (tex[3]->width - 1)) + (int)(tex_insertcord.y * (tex[3]->height - 1)) * tex[3]->width) * tex[3]->bytespp];
			norm.y = tex[3]->data[((int)(tex_insertcord.x * (tex[3]->width - 1)) + (int)(tex_insertcord.y * (tex[3]->height - 1)) * tex[3]->width) * tex[3]->bytespp+1];
			norm.z = tex[3]->data[((int)(tex_insertcord.x * (tex[3]->width - 1)) + (int)(tex_insertcord.y * (tex[3]->height - 1)) * tex[3]->width) * tex[3]->bytespp+2];
			norm.t = 1;

			norm = (trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * norm;
			norm.standard();
			Vec3f norm_vec3(norm.x, norm.y, norm.z);
			norm_vec3.norm();

			Vec3f light_dir( lightPos.x - cor_insertcord.x, lightPos.y - cor_insertcord.y, lightPos.z - cor_insertcord.z);
			Vec3f observe_dir(cor_insertcord.x * -1 ,cor_insertcord.y * -1 ,cor_insertcord.z * -1 );
			Vec3f half_dir(light_dir.x + observe_dir.x, light_dir.y + observe_dir.y, light_dir.z + observe_dir.z);
			light_dir.norm();
			observe_dir.norm();
			half_dir.norm();

			float diffuse = norm_vec3 * light_dir + 0.4;
			float spec = norm_vec3 * half_dir;
			float glow = 5;
			if (diffuse < 0)diffuse= 0;
			spec= pow(max(spec, 0.0), 32);



			//std::cout << light << std::endl;
			int b = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp] * diffuse;
			int g = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp + 1] * diffuse;
			int r = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp + 2] * diffuse;

			//std::cout << r << ", " << g << ", " << b << std::endl;


			b += tex[1]->data[((int)(tex_insertcord.x * (tex[1]->width - 1)) + (int)(tex_insertcord.y * (tex[1]->height - 1)) * tex[1]->width) * tex[1]->bytespp] * spec;
			g += tex[1]->data[((int)(tex_insertcord.x * (tex[1]->width - 1)) + (int)(tex_insertcord.y * (tex[1]->height - 1)) * tex[1]->width) * tex[1]->bytespp + 1] * spec;
			r += tex[1]->data[((int)(tex_insertcord.x * (tex[1]->width - 1)) + (int)(tex_insertcord.y * (tex[1]->height - 1)) * tex[1]->width) * tex[1]->bytespp + 2] * spec;
			//std::cout << r << ", " << g << ", " << b << std::endl;


			b += tex[2]->data[((int)(tex_insertcord.x * (tex[2]->width - 1)) + (int)(tex_insertcord.y * (tex[2]->height - 1)) * tex[2]->width) * tex[2]->bytespp] * glow;
			g += tex[2]->data[((int)(tex_insertcord.x * (tex[2]->width - 1)) + (int)(tex_insertcord.y * (tex[2]->height - 1)) * tex[2]->width) * tex[2]->bytespp + 1] * glow;
			r += tex[2]->data[((int)(tex_insertcord.x * (tex[2]->width - 1)) + (int)(tex_insertcord.y * (tex[2]->height - 1)) * tex[2]->width) * tex[2]->bytespp + 2] * glow;
			//std::cout << r << ", " << g << ", " << b << std::endl;
			//std::cout <<"-----------" << std::endl;


			if (zbuffer[int(P.x + P.y * width)] < P.z) {
				zbuffer[int(P.x + P.y * width)] = P.z;
				//std::cout << zbuffer[int(P.x + P.y * height)] << std::endl;
				SetPixel(hdc, P.x, P.y, RGB(min(r, 255), min(g, 255), min(b, 255)));
			}
		}
	}
}

void rasterization_norm_shadow(Vec3f* vertex, float* zbuffer, int height, int width, HDC hdc, TGA** tex, Vec3f* texcord, Vec3f z_afterview, Vec3f lightPos, Matrix* trans, Vec3f* world_coords_afterview, float* shadowMap) {
	Vec2i boxmin((min(min(vertex[0].x, vertex[1].x), vertex[2].x)), (min(min(vertex[0].y, vertex[1].y), vertex[2].y)));
	Vec2i boxmax((max(max(vertex[0].x, vertex[1].x), vertex[2].x)), (max(max(vertex[0].y, vertex[1].y), vertex[2].y)));
	Vec3f P;
	P.z = 0;
	for (P.x = boxmin.x; P.x <= boxmax.x; P.x++) {
		for (P.y = boxmin.y; P.y <= boxmax.y; P.y++) {
			Vec3f baryPara = getBarCor(vertex, Vec2i(P.x, P.y));
			if (baryPara.x < 0 || baryPara.y < 0 || baryPara.z < 0) continue;

			Vec3f tex_insertcord;
			Vec3f insert_para;

			insert_para.x = baryPara.x / z_afterview.x;
			insert_para.y = baryPara.y / z_afterview.y;
			insert_para.z = baryPara.z / z_afterview.z;
		
			tex_insertcord.x = texcord[0].x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[1].x * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[2].x * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			tex_insertcord.y = texcord[0].y * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[1].y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ texcord[2].y * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			
			Vec3f cor_insertcord;
			cor_insertcord.x = world_coords_afterview[0].x * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[1].x * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[2].x * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			cor_insertcord.y = world_coords_afterview[0].y * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[1].y * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[2].y * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			cor_insertcord.z = world_coords_afterview[0].z * insert_para.x / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[1].z * insert_para.y / (insert_para.x + insert_para.y + insert_para.z)
				+ world_coords_afterview[2].z * insert_para.z / (insert_para.x + insert_para.y + insert_para.z);
			P.z = cor_insertcord.z;

			Vec4f cor_insert(cor_insertcord, 1);
			Vec4f changetolight = trans[3] * (trans[1].inverse()) * cor_insert;
			int screen_coords_x = width / 2 - (trans[2] * changetolight).norm().x * width / 2;
			int screen_coords_y = height / 2 + (trans[2] * changetolight).norm().y * height / 2;
			//std::cout << screen_coords_x << ", " << screen_coords_y << std::endl;



			Vec4f norm;
			norm.x = tex[3]->data[((int)(tex_insertcord.x * (tex[3]->width - 1)) + (int)(tex_insertcord.y * (tex[3]->height - 1)) * tex[3]->width) * tex[3]->bytespp];
			norm.y = tex[3]->data[((int)(tex_insertcord.x * (tex[3]->width - 1)) + (int)(tex_insertcord.y * (tex[3]->height - 1)) * tex[3]->width) * tex[3]->bytespp+1];
			norm.z = tex[3]->data[((int)(tex_insertcord.x * (tex[3]->width - 1)) + (int)(tex_insertcord.y * (tex[3]->height - 1)) * tex[3]->width) * tex[3]->bytespp+2];
			norm.t = 1;

			norm = (trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * norm;
			norm.standard();
			Vec3f norm_vec3(norm.x, norm.y, norm.z);
			norm_vec3.norm();

			Vec3f light_dir( lightPos.x - cor_insertcord.x, lightPos.y - cor_insertcord.y, lightPos.z - cor_insertcord.z);
			Vec3f observe_dir(cor_insertcord.x * -1 ,cor_insertcord.y * -1 ,cor_insertcord.z * -1 );
			Vec3f half_dir(light_dir.x + observe_dir.x, light_dir.y + observe_dir.y, light_dir.z + observe_dir.z);
			light_dir.norm();
			observe_dir.norm();
			half_dir.norm();

			float diffuse = norm_vec3 * light_dir + 0.4;
			float spec = norm_vec3 * half_dir;
			float glow = 5;
			if (diffuse < 0)diffuse= 0;
			spec= pow(max(spec, 0.0), 32);



			//std::cout << light << std::endl;
			int b = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp] * diffuse;
			int g = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp + 1] * diffuse;
			int r = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp + 2] * diffuse;

			//std::cout << r << ", " << g << ", " << b << std::endl;


			b += tex[1]->data[((int)(tex_insertcord.x * (tex[1]->width - 1)) + (int)(tex_insertcord.y * (tex[1]->height - 1)) * tex[1]->width) * tex[1]->bytespp] * spec;
			g += tex[1]->data[((int)(tex_insertcord.x * (tex[1]->width - 1)) + (int)(tex_insertcord.y * (tex[1]->height - 1)) * tex[1]->width) * tex[1]->bytespp + 1] * spec;
			r += tex[1]->data[((int)(tex_insertcord.x * (tex[1]->width - 1)) + (int)(tex_insertcord.y * (tex[1]->height - 1)) * tex[1]->width) * tex[1]->bytespp + 2] * spec;
			//std::cout << r << ", " << g << ", " << b << std::endl;


			b += tex[2]->data[((int)(tex_insertcord.x * (tex[2]->width - 1)) + (int)(tex_insertcord.y * (tex[2]->height - 1)) * tex[2]->width) * tex[2]->bytespp] * glow;
			g += tex[2]->data[((int)(tex_insertcord.x * (tex[2]->width - 1)) + (int)(tex_insertcord.y * (tex[2]->height - 1)) * tex[2]->width) * tex[2]->bytespp + 1] * glow;
			r += tex[2]->data[((int)(tex_insertcord.x * (tex[2]->width - 1)) + (int)(tex_insertcord.y * (tex[2]->height - 1)) * tex[2]->width) * tex[2]->bytespp + 2] * glow;
			//std::cout << r << ", " << g << ", " << b << std::endl;
			//std::cout <<"-----------" << std::endl;


			if (zbuffer[int(P.x + P.y * width)] < P.z) {
				zbuffer[int(P.x + P.y * width)] = P.z;
				//std::cout << zbuffer[int(P.x + P.y * height)] << std::endl;
				if (shadowMap[int(screen_coords_x + screen_coords_y * width)] > changetolight.z) {
					int b = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp] * 0.4;
					int g = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp + 1] * 0.4;
					int r = tex[0]->data[((int)(tex_insertcord.x * (tex[0]->width - 1)) + (int)(tex_insertcord.y * (tex[0]->height - 1)) * tex[0]->width) * tex[0]->bytespp + 2] * 0.4;
					SetPixel(hdc, P.x, P.y, RGB(min(r, 255), min(g, 255), min(b, 255)));
					continue;
				}
				SetPixel(hdc, P.x, P.y, RGB(min(r, 255), min(g, 255), min(b, 255)));
			}
		}
	}
}