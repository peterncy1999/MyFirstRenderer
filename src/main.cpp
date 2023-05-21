#include "head.h"


int main() {
	Window* win = new Window(1000, 1000, "tE");

	//Vec2i t0[3] = { Vec2i(0, 0),   Vec2i(300, 0),  Vec2i(150, 300) };
	//Vec2i t1[3] = { Vec2i(400, 0),  Vec2i(700, 0),   Vec2i(370, 680) };
	//Vec2i t2[3] = { Vec2i(980, 950), Vec2i(820, 960), Vec2i(830, 780) };
	//Vec2i t3[3] = { Vec2i(800, 0),  Vec2i(800, 400),   Vec2i(900,800) };
	//Vec2i t4[3] = { Vec2i(680, 650), Vec2i(620, 660), Vec2i(630, 680) };
	//Vec2i t5[3] = { Vec2i(99, 65), Vec2i(420, 660), Vec2i(630, 680) };
	//triangle_barycentric(t0, win->screenHDC, 255, 0, 0);
	//triangle_barycentric(t1, win->screenHDC, 0, 255, 0);
	//triangle_barycentric(t2, win->screenHDC, 0, 0, 255);
	//triangle_barycentric(t3, win->screenHDC, 255, 0, 255);
	//triangle_barycentric(t4, win->screenHDC, 0, 255, 255);
	//triangle_barycentric(t5, win->screenHDC, 255, 255, 0);
	//Model obj("D:/african_head.obj");
    //Model testobj("D:/diablo3_pose.obj");

	Matrix model_magnify;
	model_magnify.val[0][0] = 500;
	model_magnify.val[1][1] = 500;
	model_magnify.val[2][2] = 500;
	model_magnify.val[3][3] = 1;
	Matrix model_move;
	model_move.val[0][0] = 1;
	model_move.val[1][1] = 1;
	model_move.val[2][2] = 1;
	model_move.val[2][3] = -1000;
	model_move.val[3][3] = 1;
	Matrix Model = model_move * model_magnify;
	//Matrix Viewc = view(Vec3f(0, 1000, 0), Vec3f(0, -0.7, -0.7), Vec3f(0, 0.7, -0.7));
	//Matrix Projection = projection(-300, -2500, 60, 1);

	Matrix floor_move;
	floor_move.val[0][0] = 1;
	floor_move.val[1][1] = 1;
	floor_move.val[2][2] = 1;
	floor_move.val[2][3] = -2000;
	floor_move.val[3][3] = 1;

	Matrix floor_rotate;
	floor_rotate.val[0][0] = 1;
	floor_rotate.val[1][1] = cos(270 * acos(-1) / 180);
	floor_rotate.val[1][2] = -sin(270 * acos(-1) / 180);
	floor_rotate.val[2][1] = sin(270 * acos(-1) / 180);
	floor_rotate.val[2][2] = cos(270 * acos(-1) / 180);
	floor_rotate.val[3][3] = 1;


	Matrix floor_magnify;
	floor_magnify.val[0][0] = 600;
	floor_magnify.val[1][1] = 600;
	floor_magnify.val[2][2] = 600;
	floor_magnify.val[3][3] = 1;

	Matrix Model2 = floor_move * floor_rotate * floor_magnify;

	Matrix Viewc = view(Vec3f(0, 0, -50), Vec3f(0, 0, -1), Vec3f(0, 1, 0));
	Matrix lightView = view(Vec3f(-400, 400, 0), Vec3f(400, -400, -1000), (Vec3f(400, -400, -1000)^Vec3f(0, 1, 0))^ Vec3f(400, -400, -1500));
	Matrix Projection = projection(-450, -2050, 60, 1);


	Matrix Transform[4] = { Model, Viewc, Projection , lightView};
	Matrix Transform2[3] = { Model2, Viewc, Projection };
	float* zbuffer = new float[win->width * win->height];
	for (int i = 0; i < win->width * win->height; ++i)  zbuffer[i] = std::numeric_limits<float>::lowest();
	float* shadowMap = new float[win->width * win->height];
	for (int i = 0; i < win->width * win->height; ++i)  zbuffer[i] = std::numeric_limits<float>::lowest();
	std::vector<std::string> tex2= {"D:/floor_diffuse.tga"};
	std::vector<std::string> tex = { "D:/diablo3_pose_diffuse.tga", "D:/diablo3_pose_spec.tga", "D:/diablo3_pose_glow.tga", "D:/diablo3_pose_nm.tga" };

	Vec3f lightPos(-400, 400, 0);

	Render rendertest;
	//rendertest.renderobj("D:/african_head.obj","D:/african_head_diffuse.tga" ,win->height, win->width, Transform, win->screenHDC);
	//rendertest.renderobj("D:/diablo3_pose.obj", tex, win->height, win->width, zbuffer, Transform, win->screenHDC);
	//rendertest.renderobj("D:/floor.obj", tex, win->height, win->width, zbuffer,Transform, win->screenHDC);
	//rendertest.renderobj_diffuse("D:/floor.obj", tex2, win->height, win->width, zbuffer, Transform2, win->screenHDC);
	//rendertest.renderobj_norm("D:/diablo3_pose.obj", tex, win->height, win->width, zbuffer, Transform, win->screenHDC);
	rendertest.get_shadowmap("D:/diablo3_pose.obj", win->height, win->width, shadowMap, Transform, lightPos, win->screenHDC);
	rendertest.renderwithshadow_norm("D:/diablo3_pose.obj", tex, win->height, win->width, zbuffer, shadowMap, lightPos, Transform, win->screenHDC);

	win->show();
	while (1) {
		win->update();
	}
	system("pause");
}