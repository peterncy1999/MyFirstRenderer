#include "Render.h"


void Render::renderobj(const char* filename, std::vector<std::string> texname, int height, int width, float* zbuffer, Matrix* trans,  HDC hdc) {
	TGA* tex_diffuse = new TGA();
	tex_diffuse->read_tga_file (texname[0].c_str());
	tex_diffuse->flip_vertically();
	std::cout << texname[0].c_str() << std::endl;
	TGA* tex_spec = new TGA();
	tex_spec->read_tga_file(texname[1].c_str());
	tex_spec->flip_vertically();
	std::cout << texname[1].c_str() << std::endl;
	TGA* tex_glow = new TGA();
	tex_glow->read_tga_file(texname[2].c_str());
	tex_glow->flip_vertically();
	std::cout << texname[2].c_str() << std::endl;
	TGA* tex[3] = { tex_diffuse, tex_spec, tex_glow };

	obj = new Model(filename);

	Vec3f light_pos(0, -1000, 500);

	for (auto i : obj->f) {
		Vec3f av = obj->v[i[0]];
		Vec3f bv = obj->v[i[3]];
		Vec3f cv = obj->v[i[6]];
		Vec3f at = obj->vt[i[1]];
		Vec3f bt = obj->vt[i[4]];
		Vec3f ct = obj->vt[i[7]];
		Vec3f an = obj->vn[i[2]];
		Vec3f bn = obj->vn[i[5]];
		Vec3f cn = obj->vn[i[8]];

		Vec4f world_coords[3] = {
			trans[1] * trans[0] * Vec4f(av, 1),
			trans[1] * trans[0] * Vec4f(bv, 1),
			trans[1] * trans[0] * Vec4f(cv, 1)
		};//MV
		Vec4f norm_coords[3] = {
			(trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * Vec4f(an, 1),
			(trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * Vec4f(bn, 1),
			(trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * Vec4f(cn, 1)
		};//MV
		//std::cout << norm_coords[0].x << ", " << norm_coords[0].y << ", " << norm_coords[0].z << ", " << norm_coords[0].t << std::endl;
		for (int i = 0; i < 3; i++) {
			norm_coords[0].standard();
			norm_coords[1].standard();
			norm_coords[2].standard();
		}
		//std::cout << norm_coords[0].x << ", " << norm_coords[0].y << ", " << norm_coords[0].z << ", " << norm_coords[0].t << std::endl;
		Vec3f norm_coords_vec3[3] = {
			Vec3f(norm_coords[0].x, norm_coords[0].y, norm_coords[0].z),
			Vec3f(norm_coords[1].x, norm_coords[1].y, norm_coords[1].z),
			Vec3f(norm_coords[2].x, norm_coords[2].y, norm_coords[2].z)
		};

		for (int i = 0; i < 3; i++) {
			norm_coords_vec3[0].norm();
			norm_coords_vec3[1].norm();
			norm_coords_vec3[2].norm();
		}
		//std::cout << norm_coords_vec3[0].x << ", " << norm_coords_vec3[0].y << ", " << norm_coords_vec3[0].z << std::endl;
		Vec3f tex_coords[3] = { at, bt, ct };
		Vec3f world_coords_afterview[3] = {
			Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z),
			Vec3f(world_coords[1].x, world_coords[1].y, world_coords[1].z),
			Vec3f(world_coords[2].x, world_coords[2].y, world_coords[2].z)
		};

		Vec3f z_afterview(world_coords[0].z, world_coords[1].z, world_coords[2].z);

		world_coords[0] = trans[2] * world_coords[0];
		world_coords[1] = trans[2] * world_coords[1];
		world_coords[2] = trans[2] * world_coords[2];
		
		for (int i = 0; i < 3; i++) {
			world_coords[0].standard();
			world_coords[1].standard();
			world_coords[2].standard();
		}
		//P
		//std::cout << world_coords[0].x << ", " << world_coords[0].y << ", " << world_coords[0].z << std::endl;
		//std::cout << "------------" << std::endl;
		Vec3f screen_coords[3] = { 
			Vec3f(width / 2 - world_coords[0].x * width / 2 , world_coords[0].y * height / 2 + height / 2, world_coords[0].z),
			Vec3f(width / 2 - world_coords[1].x * width / 2 , world_coords[1].y * height / 2 + height / 2, world_coords[1].z),
			Vec3f(width / 2 - world_coords[2].x * width / 2 , world_coords[2].y * height / 2 + height / 2, world_coords[2].z)
		};
		//std::cout << screen_coords[0].x << ", " << screen_coords[0].y << ", " << screen_coords[0].z << std::endl;
		//std::cout << screen_coords[1].x << ", " << screen_coords[1].y << ", " << screen_coords[1].z << std::endl;
		//std::cout << screen_coords[2].x << ", " << screen_coords[2].y << ", " << screen_coords[2].z << std::endl;

		if (screen_coords[0].x > width || screen_coords[1].x > width || screen_coords[2].x > width || screen_coords[0].y > height || screen_coords[1].y > height || screen_coords[2].y > height) continue;
		if (screen_coords[0].z> 1 || screen_coords[0].z < -1 || screen_coords[1].z> 1 || screen_coords[1].z < -1 || screen_coords[2].z> 1 || screen_coords[2].z < -1) continue;
		

		
		/*Vec3f n = (Vec3f(world_coords[2].x, world_coords[2].y, world_coords[2].z) - Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z))
			^ (Vec3f(world_coords[1].x, world_coords[1].y, world_coords[1].z) - Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z));
		n.norm();
		float intensity = n * light_dir;*/
		
		////light_calculate
		//trans[1].show();
		//std::cout << trans[1] * Vec4f(light_pos, 1)<< std::endl;
		Vec3f light_pos_new = Vec3f((trans[1]  * Vec4f(light_pos, 1)).x, (trans[1]  * Vec4f(light_pos, 1)).y, (trans[1]  * Vec4f(light_pos, 1)).z);
		//std::cout << light_pos_new.x << ", " << light_pos_new.y << ", " << light_pos_new.z << std::endl;
		Vec3f light_dir[3] = { light_pos_new - world_coords_afterview[0], light_pos_new - world_coords_afterview[1] , light_pos_new - world_coords_afterview[2] };
		Vec3f observe_dir[3] = {world_coords_afterview[0] * -1 ,world_coords_afterview[1] * -1 ,world_coords_afterview[2] * -1 };
		Vec3f half_dir[3] = {
			(light_dir[0] + observe_dir[0]).norm(),
			(light_dir[1] + observe_dir[1]).norm(),
			(light_dir[2] + observe_dir[2]).norm()
		};
		//std::cout << light_dir[0] << ", " << light_dir[1] << ", " << light_dir[2] << std::endl;
		for (int i = 0; i < 3; i++) {
			light_dir[0].norm();
			light_dir[1].norm();
			light_dir[2].norm();
		}
		Vec3f diffuse(norm_coords_vec3[0] * light_dir[0], norm_coords_vec3[1] * light_dir[1], norm_coords_vec3[2] * light_dir[2]);
		Vec3f spec(norm_coords_vec3[0] * half_dir[0], norm_coords_vec3[1] * half_dir[1], norm_coords_vec3[2] * half_dir[2]);
		Vec3f glow(5, 5, 5);
		if (diffuse.x < 0)diffuse.x = 0;
		if (diffuse.y < 0)diffuse.y = 0;
		if (diffuse.z < 0)diffuse.z = 0;

		spec.x = pow(max(spec.x, 0.0), 32);
		spec.y = pow(max(spec.y, 0.0), 32);
		spec.z = pow(max(spec.z, 0.0), 32);

		Vec3f identity[3] = { diffuse, spec, glow };
		//std::cout << diffuse.x << ", " << diffuse.y << ", " << diffuse.z << std::endl;
		//std::cout << spec.x << ", " << spec.y << ", " << spec.z << std::endl;
		/*
		int b[3] = {
			tex->data[((int)(at.x * (tex->width - 1)) + (int)(at.y * (tex->height - 1)) * tex->width) * tex->bytespp] * intensity,
			tex->data[((int)(bt.x * (tex->width - 1)) + (int)(bt.y * (tex->height - 1)) * tex->width) * tex->bytespp] * intensity,
			tex->data[((int)(ct.x * (tex->width - 1)) + (int)(ct.y * (tex->height - 1)) * tex->width) * tex->bytespp] * intensity
		};
		int g[3] = {
			tex->data[((int)(at.x * (tex->width - 1)) + (int)(at.y * (tex->height - 1)) * tex->width) * tex->bytespp + 1] * intensity,
			tex->data[((int)(bt.x * (tex->width - 1)) + (int)(bt.y * (tex->height - 1)) * tex->width) * tex->bytespp + 1] * intensity,
			tex->data[((int)(ct.x * (tex->width - 1)) + (int)(ct.y * (tex->height - 1)) * tex->width) * tex->bytespp + 1] * intensity
		};
		int r[3] = {
			tex->data[((int)(at.x * (tex->width - 1)) + (int)(at.y * (tex->height - 1)) * tex->width) * tex->bytespp + 2] * intensity,
			tex->data[((int)(bt.x * (tex->width - 1)) + (int)(bt.y * (tex->height - 1)) * tex->width) * tex->bytespp + 2] * intensity,
			tex->data[((int)(ct.x * (tex->width - 1)) + (int)(ct.y * (tex->height - 1)) * tex->width) * tex->bytespp + 2] * intensity
		};
		*/
		//std::cout << tex[0]->data[1] << ", " << tex[1]->data[1] << ", " << tex[2]->data[1] << std::endl;

		//triangle_barycentric(screen_coords, zbuffer ,height, width, hdc, r, g, b);
		triangle_barycentric(screen_coords, zbuffer, height, width, hdc, tex, tex_coords, z_afterview, identity);

	}


}

void Render::renderobj_diffuse(const char* filename, std::vector<std::string> texname, int height, int width, float* zbuffer, Matrix* trans, HDC hdc) {
	TGA* tex_diffuse = new TGA();
	tex_diffuse->read_tga_file(texname[0].c_str());
	tex_diffuse->flip_vertically();
	std::cout << texname[0].c_str() << std::endl;

	obj = new Model(filename);

	Vec3f light_pos(-1000, 1000, 500);

	for (auto i : obj->f) {
		Vec3f av = obj->v[i[0]];
		Vec3f bv = obj->v[i[3]];
		Vec3f cv = obj->v[i[6]];
		Vec3f at = obj->vt[i[1]];
		Vec3f bt = obj->vt[i[4]];
		Vec3f ct = obj->vt[i[7]];
		Vec3f an = obj->vn[i[2]];
		Vec3f bn = obj->vn[i[5]];
		Vec3f cn = obj->vn[i[8]];

		Vec4f world_coords[3] = {
			trans[1] * trans[0] * Vec4f(av, 1),
			trans[1] * trans[0] * Vec4f(bv, 1),
			trans[1] * trans[0] * Vec4f(cv, 1)
		};//MV
		Vec4f norm_coords[3] = {
			(trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * Vec4f(an, 1),
			(trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * Vec4f(bn, 1),
			(trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * Vec4f(cn, 1)
		};//MV
		//std::cout << norm_coords[0].x << ", " << norm_coords[0].y << ", " << norm_coords[0].z << ", " << norm_coords[0].t << std::endl;
		for (int i = 0; i < 3; i++) {
			norm_coords[0].standard();
			norm_coords[1].standard();
			norm_coords[2].standard();
		}
		//std::cout << norm_coords[0].x << ", " << norm_coords[0].y << ", " << norm_coords[0].z << ", " << norm_coords[0].t << std::endl;
		Vec3f norm_coords_vec3[3] = {
			Vec3f(norm_coords[0].x, norm_coords[0].y, norm_coords[0].z),
			Vec3f(norm_coords[1].x, norm_coords[1].y, norm_coords[1].z),
			Vec3f(norm_coords[2].x, norm_coords[2].y, norm_coords[2].z)
		};

		for (int i = 0; i < 3; i++) {
			norm_coords_vec3[0].norm();
			norm_coords_vec3[1].norm();
			norm_coords_vec3[2].norm();
		}
		//std::cout << norm_coords_vec3[0].x << ", " << norm_coords_vec3[0].y << ", " << norm_coords_vec3[0].z << std::endl;
		Vec3f tex_coords[3] = { at, bt, ct };
		Vec3f world_coords_afterview[3] = {
			Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z),
			Vec3f(world_coords[1].x, world_coords[1].y, world_coords[1].z),
			Vec3f(world_coords[2].x, world_coords[2].y, world_coords[2].z)
		};

		Vec3f z_afterview(world_coords[0].z, world_coords[1].z, world_coords[2].z);

		world_coords[0] = trans[2] * world_coords[0];
		world_coords[1] = trans[2] * world_coords[1];
		world_coords[2] = trans[2] * world_coords[2];

		for (int i = 0; i < 3; i++) {
			world_coords[0].standard();
			world_coords[1].standard();
			world_coords[2].standard();
		}
		//P
		//std::cout << world_coords[0].x << ", " << world_coords[0].y << ", " << world_coords[0].z << std::endl;
		//std::cout << "------------" << std::endl;
		Vec3f screen_coords[3] = {
			Vec3f(width / 2 - world_coords[0].x * width / 2 , world_coords[0].y * height / 2 + height / 2, world_coords[0].z),
			Vec3f(width / 2 - world_coords[1].x * width / 2 , world_coords[1].y * height / 2 + height / 2, world_coords[1].z),
			Vec3f(width / 2 - world_coords[2].x * width / 2 , world_coords[2].y * height / 2 + height / 2, world_coords[2].z)
		};
		//std::cout << screen_coords[0].x << ", " << screen_coords[0].y << ", " << screen_coords[0].z << std::endl;
		//std::cout << screen_coords[1].x << ", " << screen_coords[1].y << ", " << screen_coords[1].z << std::endl;
		//std::cout << screen_coords[2].x << ", " << screen_coords[2].y << ", " << screen_coords[2].z << std::endl;

		if (screen_coords[0].x > width || screen_coords[1].x > width || screen_coords[2].x > width || screen_coords[0].y > height || screen_coords[1].y > height || screen_coords[2].y > height) continue;
		if (screen_coords[0].z > 1 || screen_coords[0].z < -1 || screen_coords[1].z> 1 || screen_coords[1].z < -1 || screen_coords[2].z> 1 || screen_coords[2].z < -1) continue;



		/*Vec3f n = (Vec3f(world_coords[2].x, world_coords[2].y, world_coords[2].z) - Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z))
			^ (Vec3f(world_coords[1].x, world_coords[1].y, world_coords[1].z) - Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z));
		n.norm();
		float intensity = n * light_dir;*/

		////light_calculate
		//trans[1].show();
		//std::cout << trans[1] * Vec4f(light_pos, 1)<< std::endl;
		Vec3f light_pos_new = Vec3f((trans[1] * Vec4f(light_pos, 1)).x, (trans[1] * Vec4f(light_pos, 1)).y, (trans[1] * Vec4f(light_pos, 1)).z);
		//std::cout << light_pos_new.x << ", " << light_pos_new.y << ", " << light_pos_new.z << std::endl;
		Vec3f light_dir[3] = { light_pos_new - world_coords_afterview[0], light_pos_new - world_coords_afterview[1] , light_pos_new - world_coords_afterview[2] };
		Vec3f observe_dir[3] = { world_coords_afterview[0] * -1 ,world_coords_afterview[1] * -1 ,world_coords_afterview[2] * -1 };
		
		
		Vec3f diffuse(norm_coords_vec3[0] * light_dir[0], norm_coords_vec3[1] * light_dir[1], norm_coords_vec3[2] * light_dir[2]);
		
		if (diffuse.x < 0)diffuse.x = 0;
		if (diffuse.y < 0)diffuse.y = 0;
		if (diffuse.z < 0)diffuse.z = 0;


		//std::cout << diffuse.x << ", " << diffuse.y << ", " << diffuse.z << std::endl;
		//std::cout << spec.x << ", " << spec.y << ", " << spec.z << std::endl;
		/*
		int b[3] = {
			tex->data[((int)(at.x * (tex->width - 1)) + (int)(at.y * (tex->height - 1)) * tex->width) * tex->bytespp] * intensity,
			tex->data[((int)(bt.x * (tex->width - 1)) + (int)(bt.y * (tex->height - 1)) * tex->width) * tex->bytespp] * intensity,
			tex->data[((int)(ct.x * (tex->width - 1)) + (int)(ct.y * (tex->height - 1)) * tex->width) * tex->bytespp] * intensity
		};
		int g[3] = {
			tex->data[((int)(at.x * (tex->width - 1)) + (int)(at.y * (tex->height - 1)) * tex->width) * tex->bytespp + 1] * intensity,
			tex->data[((int)(bt.x * (tex->width - 1)) + (int)(bt.y * (tex->height - 1)) * tex->width) * tex->bytespp + 1] * intensity,
			tex->data[((int)(ct.x * (tex->width - 1)) + (int)(ct.y * (tex->height - 1)) * tex->width) * tex->bytespp + 1] * intensity
		};
		int r[3] = {
			tex->data[((int)(at.x * (tex->width - 1)) + (int)(at.y * (tex->height - 1)) * tex->width) * tex->bytespp + 2] * intensity,
			tex->data[((int)(bt.x * (tex->width - 1)) + (int)(bt.y * (tex->height - 1)) * tex->width) * tex->bytespp + 2] * intensity,
			tex->data[((int)(ct.x * (tex->width - 1)) + (int)(ct.y * (tex->height - 1)) * tex->width) * tex->bytespp + 2] * intensity
		};
		*/
		//std::cout << tex[0]->data[1] << ", " << tex[1]->data[1] << ", " << tex[2]->data[1] << std::endl;

		//triangle_barycentric(screen_coords, zbuffer ,height, width, hdc, r, g, b);
		triangle_barycentric_diffuse(screen_coords, zbuffer, height, width, hdc, tex_diffuse, tex_coords, z_afterview, world_coords_afterview, diffuse);

	}


}

void Render::renderobj_norm(const char* filename, std::vector<std::string> texname, int height, int width, float* zbuffer, Matrix* trans, HDC hdc) {
	TGA* tex_diffuse = new TGA();
	tex_diffuse->read_tga_file(texname[0].c_str());
	tex_diffuse->flip_vertically();
	std::cout << texname[0].c_str() << std::endl;
	TGA* tex_spec = new TGA();
	tex_spec->read_tga_file(texname[1].c_str());
	tex_spec->flip_vertically();
	std::cout << texname[1].c_str() << std::endl;
	TGA* tex_glow = new TGA();
	tex_glow->read_tga_file(texname[2].c_str());
	tex_glow->flip_vertically();
	std::cout << texname[2].c_str() << std::endl;
	TGA* tex_norm = new TGA();
	tex_norm->read_tga_file(texname[3].c_str());
	tex_norm->flip_vertically();
	TGA* tex[4] = { tex_diffuse, tex_spec, tex_glow , tex_norm};
	obj = new Model(filename);

	Vec3f light_pos(-1000, 1000, 500);

	for (auto i : obj->f) {
		Vec3f av = obj->v[i[0]];
		Vec3f bv = obj->v[i[3]];
		Vec3f cv = obj->v[i[6]];
		Vec3f at = obj->vt[i[1]];
		Vec3f bt = obj->vt[i[4]];
		Vec3f ct = obj->vt[i[7]];


		Vec4f world_coords[3] = {
			trans[1] * trans[0] * Vec4f(av, 1),
			trans[1] * trans[0] * Vec4f(bv, 1),
			trans[1] * trans[0] * Vec4f(cv, 1)
		};//MV

		//std::cout << norm_coords_vec3[0].x << ", " << norm_coords_vec3[0].y << ", " << norm_coords_vec3[0].z << std::endl;
		Vec3f tex_coords[3] = { at, bt, ct };
		Vec3f world_coords_afterview[3] = {
			Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z),
			Vec3f(world_coords[1].x, world_coords[1].y, world_coords[1].z),
			Vec3f(world_coords[2].x, world_coords[2].y, world_coords[2].z)
		};

		Vec3f z_afterview(world_coords[0].z, world_coords[1].z, world_coords[2].z);

		world_coords[0] = trans[2] * world_coords[0];
		world_coords[1] = trans[2] * world_coords[1];
		world_coords[2] = trans[2] * world_coords[2];

		for (int i = 0; i < 3; i++) {
			world_coords[0].standard();
			world_coords[1].standard();
			world_coords[2].standard();
		}
		//P
		//std::cout << world_coords[0].x << ", " << world_coords[0].y << ", " << world_coords[0].z << std::endl;
		//std::cout << "------------" << std::endl;
		Vec3f screen_coords[3] = {
			Vec3f(width / 2 - world_coords[0].x * width / 2 , world_coords[0].y * height / 2 + height / 2, world_coords[0].z),
			Vec3f(width / 2 - world_coords[1].x * width / 2 , world_coords[1].y * height / 2 + height / 2, world_coords[1].z),
			Vec3f(width / 2 - world_coords[2].x * width / 2 , world_coords[2].y * height / 2 + height / 2, world_coords[2].z)
		};
		//std::cout << screen_coords[0].x << ", " << screen_coords[0].y << ", " << screen_coords[0].z << std::endl;
		//std::cout << screen_coords[1].x << ", " << screen_coords[1].y << ", " << screen_coords[1].z << std::endl;
		//std::cout << screen_coords[2].x << ", " << screen_coords[2].y << ", " << screen_coords[2].z << std::endl;

		if (screen_coords[0].x > width || screen_coords[1].x > width || screen_coords[2].x > width || screen_coords[0].y > height || screen_coords[1].y > height || screen_coords[2].y > height) continue;
		if (screen_coords[0].z > 1 || screen_coords[0].z < -1 || screen_coords[1].z> 1 || screen_coords[1].z < -1 || screen_coords[2].z> 1 || screen_coords[2].z < -1) continue;

		Vec3f light_pos_new = Vec3f((trans[1] * Vec4f(light_pos, 1)).x, (trans[1] * Vec4f(light_pos, 1)).y, (trans[1] * Vec4f(light_pos, 1)).z);
		triangle_barycentric_norm(screen_coords, zbuffer, height, width, hdc, tex, tex_coords, z_afterview, light_pos_new, trans, world_coords_afterview);

	}


}

void Render::get_shadowmap(const char* filename, int height, int width, float* zbuffer, Matrix* trans, Vec3f light_pos ,HDC hdc) {
	
	obj = new Model(filename);


	for (auto i : obj->f) {
		Vec3f av = obj->v[i[0]];
		Vec3f bv = obj->v[i[3]];
		Vec3f cv = obj->v[i[6]];


		Vec4f world_coords[3] = {
			trans[3] * trans[0] * Vec4f(av, 1),
			trans[3] * trans[0] * Vec4f(bv, 1),
			trans[3] * trans[0] * Vec4f(cv, 1)
		};//MV

		//std::cout << norm_coords_vec3[0].x << ", " << norm_coords_vec3[0].y << ", " << norm_coords_vec3[0].z << std::endl;

		Vec3f world_coords_afterview[3] = {
			Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z),
			Vec3f(world_coords[1].x, world_coords[1].y, world_coords[1].z),
			Vec3f(world_coords[2].x, world_coords[2].y, world_coords[2].z)
		};



		world_coords[0] = trans[2] * world_coords[0];
		world_coords[1] = trans[2] * world_coords[1];
		world_coords[2] = trans[2] * world_coords[2];

		for (int i = 0; i < 3; i++) {
			world_coords[0].standard();
			world_coords[1].standard();
			world_coords[2].standard();
		}
		//P
		//std::cout << world_coords[0].x << ", " << world_coords[0].y << ", " << world_coords[0].z << std::endl;
		//std::cout << "------------" << std::endl;
		Vec3f screen_coords[3] = {
			Vec3f(width / 2 - world_coords[0].x * width / 2 , world_coords[0].y * height / 2 + height / 2, world_coords[0].z),
			Vec3f(width / 2 - world_coords[1].x * width / 2 , world_coords[1].y * height / 2 + height / 2, world_coords[1].z),
			Vec3f(width / 2 - world_coords[2].x * width / 2 , world_coords[2].y * height / 2 + height / 2, world_coords[2].z)
		};
		//std::cout << screen_coords[0].x << ", " << screen_coords[0].y << ", " << screen_coords[0].z << std::endl;
		//std::cout << screen_coords[1].x << ", " << screen_coords[1].y << ", " << screen_coords[1].z << std::endl;
		//std::cout << screen_coords[2].x << ", " << screen_coords[2].y << ", " << screen_coords[2].z << std::endl;

		if (screen_coords[0].x > width || screen_coords[1].x > width || screen_coords[2].x > width || screen_coords[0].y > height || screen_coords[1].y > height || screen_coords[2].y > height) continue;
		if (screen_coords[0].z > 1 || screen_coords[0].z < -1 || screen_coords[1].z> 1 || screen_coords[1].z < -1 || screen_coords[2].z> 1 || screen_coords[2].z < -1) continue;
		
		Vec2i boxmin((min(min(screen_coords[0].x, screen_coords[1].x), screen_coords[2].x)), (min(min(screen_coords[0].y, screen_coords[1].y), screen_coords[2].y)));
		Vec2i boxmax((max(max(screen_coords[0].x, screen_coords[1].x), screen_coords[2].x)), (max(max(screen_coords[0].y, screen_coords[1].y), screen_coords[2].y)));
		Vec3f P;
		P.z = 0;
		for (P.x = boxmin.x; P.x <= boxmax.x; P.x++) {
			for (P.y = boxmin.y; P.y <= boxmax.y; P.y++) {
				Vec3f baryPara = getBarCor(screen_coords, Vec2i(P.x, P.y));
				if (baryPara.x < 0 || baryPara.y < 0 || baryPara.z < 0) continue;
				P.z = 0;

				Vec3f tex_insertcord;
				Vec3f insert_para;
				insert_para.x = baryPara.x / world_coords_afterview[0].z;
				insert_para.y = baryPara.y / world_coords_afterview[1].z;
				insert_para.z = baryPara.z / world_coords_afterview[2].z;


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
				}
			}
		}

	}


}

void Render::renderwithshadow_norm(const char* filename, std::vector<std::string> texname, int height, int width, float* zbuffer, float* shadowMap, Vec3f light_pos, Matrix* trans, HDC hdc) {
	TGA* tex_diffuse = new TGA();
	tex_diffuse->read_tga_file(texname[0].c_str());
	tex_diffuse->flip_vertically();
	std::cout << texname[0].c_str() << std::endl;
	TGA* tex_spec = new TGA();
	tex_spec->read_tga_file(texname[1].c_str());
	tex_spec->flip_vertically();
	std::cout << texname[1].c_str() << std::endl;
	TGA* tex_glow = new TGA();
	tex_glow->read_tga_file(texname[2].c_str());
	tex_glow->flip_vertically();
	std::cout << texname[2].c_str() << std::endl;
	TGA* tex_norm = new TGA();
	tex_norm->read_tga_file(texname[3].c_str());
	tex_norm->flip_vertically();
	TGA* tex[4] = { tex_diffuse, tex_spec, tex_glow , tex_norm };
	obj = new Model(filename);

	for (auto i : obj->f) {
		Vec3f av = obj->v[i[0]];
		Vec3f bv = obj->v[i[3]];
		Vec3f cv = obj->v[i[6]];
		Vec3f at = obj->vt[i[1]];
		Vec3f bt = obj->vt[i[4]];
		Vec3f ct = obj->vt[i[7]];


		Vec4f world_coords[3] = {
			trans[1] * trans[0] * Vec4f(av, 1),
			trans[1] * trans[0] * Vec4f(bv, 1),
			trans[1] * trans[0] * Vec4f(cv, 1)
		};//MV

		//std::cout << norm_coords_vec3[0].x << ", " << norm_coords_vec3[0].y << ", " << norm_coords_vec3[0].z << std::endl;
		Vec3f tex_coords[3] = { at, bt, ct };
		Vec3f world_coords_afterview[3] = {
			Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z),
			Vec3f(world_coords[1].x, world_coords[1].y, world_coords[1].z),
			Vec3f(world_coords[2].x, world_coords[2].y, world_coords[2].z)
		};

		Vec3f z_afterview(world_coords[0].z, world_coords[1].z, world_coords[2].z);

		world_coords[0] = trans[2] * world_coords[0];
		world_coords[1] = trans[2] * world_coords[1];
		world_coords[2] = trans[2] * world_coords[2];

		for (int i = 0; i < 3; i++) {
			world_coords[0].standard();
			world_coords[1].standard();
			world_coords[2].standard();
		}
		//P
		//std::cout << world_coords[0].x << ", " << world_coords[0].y << ", " << world_coords[0].z << std::endl;
		//std::cout << "------------" << std::endl;
		Vec3f screen_coords[3] = {
			Vec3f(width / 2 - world_coords[0].x * width / 2 , world_coords[0].y * height / 2 + height / 2, world_coords[0].z),
			Vec3f(width / 2 - world_coords[1].x * width / 2 , world_coords[1].y * height / 2 + height / 2, world_coords[1].z),
			Vec3f(width / 2 - world_coords[2].x * width / 2 , world_coords[2].y * height / 2 + height / 2, world_coords[2].z)
		};
		//std::cout << screen_coords[0].x << ", " << screen_coords[0].y << ", " << screen_coords[0].z << std::endl;
		//std::cout << screen_coords[1].x << ", " << screen_coords[1].y << ", " << screen_coords[1].z << std::endl;
		//std::cout << screen_coords[2].x << ", " << screen_coords[2].y << ", " << screen_coords[2].z << std::endl;

		if (screen_coords[0].x > width || screen_coords[1].x > width || screen_coords[2].x > width || screen_coords[0].y > height || screen_coords[1].y > height || screen_coords[2].y > height) continue;
		if (screen_coords[0].z > 1 || screen_coords[0].z < -1 || screen_coords[1].z> 1 || screen_coords[1].z < -1 || screen_coords[2].z> 1 || screen_coords[2].z < -1) continue;

		Vec3f light_pos_new = Vec3f((trans[1] * Vec4f(light_pos, 1)).x, (trans[1] * Vec4f(light_pos, 1)).y, (trans[1] * Vec4f(light_pos, 1)).z);
		rasterization_norm_shadow(screen_coords, zbuffer, height, width, hdc, tex, tex_coords, z_afterview, light_pos_new, trans, world_coords_afterview, shadowMap);
	}


}

void Render::renderwithshadow_diffuse(const char* filename, std::vector<std::string> texname, int height, int width, float* zbuffer, float* shadowMap, Vec3f light_pos, Matrix* trans, HDC hdc) {
	TGA* tex_diffuse = new TGA();
	tex_diffuse->read_tga_file(texname[0].c_str());
	tex_diffuse->flip_vertically();
	std::cout << texname[0].c_str() << std::endl;

	obj = new Model(filename);

	for (auto i : obj->f) {
		Vec3f av = obj->v[i[0]];
		Vec3f bv = obj->v[i[3]];
		Vec3f cv = obj->v[i[6]];
		Vec3f at = obj->vt[i[1]];
		Vec3f bt = obj->vt[i[4]];
		Vec3f ct = obj->vt[i[7]];
		Vec3f an = obj->vn[i[2]];
		Vec3f bn = obj->vn[i[5]];
		Vec3f cn = obj->vn[i[8]];

		Vec4f world_coords[3] = {
			trans[1] * trans[0] * Vec4f(av, 1),
			trans[1] * trans[0] * Vec4f(bv, 1),
			trans[1] * trans[0] * Vec4f(cv, 1)
		};//MV
		Vec4f lightview_coords[3] = {
			trans[3] * trans[0] * Vec4f(av, 1),
			trans[3] * trans[0] * Vec4f(bv, 1),
			trans[3] * trans[0] * Vec4f(cv, 1)
		};
		Vec3f lightview_z(lightview_coords[0].z, lightview_coords[1].z, lightview_coords[2].z);

		Vec4f norm_coords[3] = {
			(trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * Vec4f(an, 1),
			(trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * Vec4f(bn, 1),
			(trans[1].inverse().transpose()) * (trans[0].inverse().transpose()) * Vec4f(cn, 1)
		};//MV
		//std::cout << norm_coords[0].x << ", " << norm_coords[0].y << ", " << norm_coords[0].z << ", " << norm_coords[0].t << std::endl;
		for (int i = 0; i < 3; i++) {
			norm_coords[0].standard();
			norm_coords[1].standard();
			norm_coords[2].standard();
		}
		//std::cout << norm_coords[0].x << ", " << norm_coords[0].y << ", " << norm_coords[0].z << ", " << norm_coords[0].t << std::endl;
		Vec3f norm_coords_vec3[3] = {
			Vec3f(norm_coords[0].x, norm_coords[0].y, norm_coords[0].z),
			Vec3f(norm_coords[1].x, norm_coords[1].y, norm_coords[1].z),
			Vec3f(norm_coords[2].x, norm_coords[2].y, norm_coords[2].z)
		};

		for (int i = 0; i < 3; i++) {
			norm_coords_vec3[0].norm();
			norm_coords_vec3[1].norm();
			norm_coords_vec3[2].norm();
		}
		//std::cout << norm_coords_vec3[0].x << ", " << norm_coords_vec3[0].y << ", " << norm_coords_vec3[0].z << std::endl;
		Vec3f tex_coords[3] = { at, bt, ct };
		Vec3f world_coords_afterview[3] = {
			Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z),
			Vec3f(world_coords[1].x, world_coords[1].y, world_coords[1].z),
			Vec3f(world_coords[2].x, world_coords[2].y, world_coords[2].z)
		};

		Vec3f z_afterview(world_coords[0].z, world_coords[1].z, world_coords[2].z);

		world_coords[0] = trans[2] * world_coords[0];
		world_coords[1] = trans[2] * world_coords[1];
		world_coords[2] = trans[2] * world_coords[2];

		for (int i = 0; i < 3; i++) {
			world_coords[0].standard();
			world_coords[1].standard();
			world_coords[2].standard();
		}
		//P
		//std::cout << world_coords[0].x << ", " << world_coords[0].y << ", " << world_coords[0].z << std::endl;
		//std::cout << "------------" << std::endl;
		Vec3f screen_coords[3] = {
			Vec3f(width / 2 - world_coords[0].x * width / 2 , world_coords[0].y * height / 2 + height / 2, world_coords[0].z),
			Vec3f(width / 2 - world_coords[1].x * width / 2 , world_coords[1].y * height / 2 + height / 2, world_coords[1].z),
			Vec3f(width / 2 - world_coords[2].x * width / 2 , world_coords[2].y * height / 2 + height / 2, world_coords[2].z)
		};
		//std::cout << screen_coords[0].x << ", " << screen_coords[0].y << ", " << screen_coords[0].z << std::endl;
		//std::cout << screen_coords[1].x << ", " << screen_coords[1].y << ", " << screen_coords[1].z << std::endl;
		//std::cout << screen_coords[2].x << ", " << screen_coords[2].y << ", " << screen_coords[2].z << std::endl;

		if (screen_coords[0].x > width || screen_coords[1].x > width || screen_coords[2].x > width || screen_coords[0].y > height || screen_coords[1].y > height || screen_coords[2].y > height) continue;
		if (screen_coords[0].z > 1 || screen_coords[0].z < -1 || screen_coords[1].z> 1 || screen_coords[1].z < -1 || screen_coords[2].z> 1 || screen_coords[2].z < -1) continue;



		/*Vec3f n = (Vec3f(world_coords[2].x, world_coords[2].y, world_coords[2].z) - Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z))
			^ (Vec3f(world_coords[1].x, world_coords[1].y, world_coords[1].z) - Vec3f(world_coords[0].x, world_coords[0].y, world_coords[0].z));
		n.norm();
		float intensity = n * light_dir;*/

		////light_calculate
		//trans[1].show();
		//std::cout << trans[1] * Vec4f(light_pos, 1)<< std::endl;
		Vec3f light_pos_new = Vec3f((trans[1] * Vec4f(light_pos, 1)).x, (trans[1] * Vec4f(light_pos, 1)).y, (trans[1] * Vec4f(light_pos, 1)).z);
		//std::cout << light_pos_new.x << ", " << light_pos_new.y << ", " << light_pos_new.z << std::endl;
		Vec3f light_dir[3] = { light_pos_new - world_coords_afterview[0], light_pos_new - world_coords_afterview[1] , light_pos_new - world_coords_afterview[2] };
		Vec3f observe_dir[3] = { world_coords_afterview[0] * -1 ,world_coords_afterview[1] * -1 ,world_coords_afterview[2] * -1 };


		Vec3f diffuse(norm_coords_vec3[0] * light_dir[0], norm_coords_vec3[1] * light_dir[1], norm_coords_vec3[2] * light_dir[2]);

		if (diffuse.x < 0)diffuse.x = 0;
		if (diffuse.y < 0)diffuse.y = 0;
		if (diffuse.z < 0)diffuse.z = 0;


		//std::cout << diffuse.x << ", " << diffuse.y << ", " << diffuse.z << std::endl;
		//std::cout << spec.x << ", " << spec.y << ", " << spec.z << std::endl;
		/*
		int b[3] = {
			tex->data[((int)(at.x * (tex->width - 1)) + (int)(at.y * (tex->height - 1)) * tex->width) * tex->bytespp] * intensity,
			tex->data[((int)(bt.x * (tex->width - 1)) + (int)(bt.y * (tex->height - 1)) * tex->width) * tex->bytespp] * intensity,
			tex->data[((int)(ct.x * (tex->width - 1)) + (int)(ct.y * (tex->height - 1)) * tex->width) * tex->bytespp] * intensity
		};
		int g[3] = {
			tex->data[((int)(at.x * (tex->width - 1)) + (int)(at.y * (tex->height - 1)) * tex->width) * tex->bytespp + 1] * intensity,
			tex->data[((int)(bt.x * (tex->width - 1)) + (int)(bt.y * (tex->height - 1)) * tex->width) * tex->bytespp + 1] * intensity,
			tex->data[((int)(ct.x * (tex->width - 1)) + (int)(ct.y * (tex->height - 1)) * tex->width) * tex->bytespp + 1] * intensity
		};
		int r[3] = {
			tex->data[((int)(at.x * (tex->width - 1)) + (int)(at.y * (tex->height - 1)) * tex->width) * tex->bytespp + 2] * intensity,
			tex->data[((int)(bt.x * (tex->width - 1)) + (int)(bt.y * (tex->height - 1)) * tex->width) * tex->bytespp + 2] * intensity,
			tex->data[((int)(ct.x * (tex->width - 1)) + (int)(ct.y * (tex->height - 1)) * tex->width) * tex->bytespp + 2] * intensity
		};
		*/
		//std::cout << tex[0]->data[1] << ", " << tex[1]->data[1] << ", " << tex[2]->data[1] << std::endl;

		//triangle_barycentric(screen_coords, zbuffer ,height, width, hdc, r, g, b);
		triangle_barycentric_diffuse(screen_coords, zbuffer, height, width, hdc, tex_diffuse, tex_coords, z_afterview, world_coords_afterview, diffuse);

	}


}