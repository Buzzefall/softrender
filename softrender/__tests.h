// TODO: rewrite tests

//#pragma once
//
//#include "core.h"
//#include "geometry.h"
//#include "model.h"
//#include "tgaimage.h"
//
//#include <filesystem>
//#include <iostream>
//namespace fs = std::filesystem;
//
////#include "fmt/format.h"
////#include "fmt/core.h"
////#include "fmt/format-inl.h"
////#include "fmt/src/format.cc"
//
//
//class Tests {
//	struct {
//		TGAColor white	= TGAColor(255, 255, 255, 255);
//		TGAColor grey	= TGAColor(155, 155, 155, 255);
//		TGAColor red	= TGAColor(255, 0, 0,   255);
//		TGAColor green  = TGAColor(0, 255, 0,   255);
//		TGAColor blue   = TGAColor(0, 0, 255,   255);
//		TGAColor magenta = TGAColor(255, 0, 255, 255);
//		TGAColor cyan   = TGAColor(0, 255, 255, 255);
//	} colors;
//
//	Screen _screen;
//
//public:
//	Tests(): _screen(Screen()) {
//		if (!fs::exists("./tests")) {
//			fs::create_directory("tests");
//		}
//	}
//
//
//	template<int N, int M = N>
//	static void print_matrix(Mat<N, M>& mat) {
//		for (int i = 0; i < N; i++) {
//			for (int j = 0; j < M; j++) { std::cout << mat[i][j] << " "; }
//			std::cout << std::endl;
//		}
//		std::cout << std::endl;
//	}
//
//
//	template<int N>
//	static void print_vector(Vec<N>& vec) {
//		for (int j = 0; j < N; j++) { std::cout << vec[j] << " "; }
//		std::cout << std::endl << std::endl;
//	}
//
//
//	void draw_grid() {
//		for (int x = 0; x < _screen.width(); x++) {
//			if (x % 20 == 0) {
//				draw_line(Vec4(x, 0, 0, 1), Vec4(x, _screen.height() - 1, 0, 1), colors.grey, _screen);
//			}
//		}
//
//		for (int y = 0; y < _screen.height(); y++) {
//			if (y % 20 == 0) {
//				draw_line(Vec4(0, y, 0, 1), Vec4(_screen.width() - 1, y, 0, 1), colors.grey, _screen);
//			}
//		}
//	}
//
//
//	void run_all() {
//		math_tests();
//		//line_tests();
//		//triangle_tests();
//		//full_render_tests();
//	}
//
//
//	static void math_tests() {
//		std::cout << "Math tests...\n\n";
//
//		auto mat = Mat<4>{{
//			{1, 0, 0, 1},
//			{0, 5, 0, 1},
//			{0, 0, 0, 1},
//			{0, 0, 2, 1}
//		}};
//
//		Mat4 mat2 = {{
//			{10, 12, 14, 1},
//			{0,   8, 16, 1},
//			{0,   0,  6, 1},
//			{0,   0,  6, 1}
//		}};
//
//		auto mat_inv		= mat.inverse();
//		auto mat_transp		= mat.transpose();
//		auto mat_transpinv	= mat_transp.inverse();
//		auto mat2_transp	= mat2.transpose();
//		auto mat_symm		= (mat2 + mat2_transp) * 0.5;
//
//		print_matrix(mat);
//		print_matrix(mat_transp);
//		print_matrix(mat_inv);
//		print_matrix(mat_transpinv);
//		print_matrix(mat_symm);
//
//		std::cout << mat.det()				<< std::endl;
//		std::cout << mat_transp.det()		<< std::endl;
//		std::cout << mat_inv.det()			<< std::endl;
//		std::cout << mat_transpinv.det()	<< std::endl << std::endl;
//
//		auto result = Mat<4>::translation({25, -25, 25}) * Vec4(1, 1, 1, 1);
//		print_vector(result);
//
//		auto vec3 = Vec3(13, 17, 23);
//		auto vec3_embed_0 = Vec4(vec3, 0);
//		auto vec3_embed_1 = Vec4(vec3);
//		auto vec2 = Vec2(vec3);
//
//		std::cout << "vec3: ";
//		for (int i = 0; i < 3; i++) { std::cout <<  vec3[i] << " "; }
//		std::cout << std::endl << std::endl;
//
//		std::cout << "vec3_embed_0: ";
//		for (int i = 0; i < 4; i++) { std::cout << vec3_embed_0[i] << " "; }
//		std::cout << std::endl << std::endl;
//
//		std::cout << "vec3_embed_1: ";
//		for (int i = 0; i < 4; i++) { std::cout << vec3_embed_1[i] << " "; }
//		std::cout << std::endl << std::endl;
//
//		std::cout << "vec2_projected: ";
//		for (int i = 0; i < 2; i++) { std::cout << vec2[i] << " "; }
//		std::cout << std::endl << std::endl;
//
//		std::cout << "Math tests done."	<< std::endl << std::endl;
//
//		Vec2 pts[3] = {{0, 0}, {13, 5}, {15, 0}};
//		Vec2 P = {14, 3};
//		auto sum = [&](Vec3& v) { return v.x + v.y + v.z; };
//
//		auto b0 = barycentric0(pts, P);
//		auto b1 = barycentric(pts, P);
//
//		Mat3 m = {{ {0, 0, 1}, {13, 5, 1}, {15, 0, 1} }};
//
//		std::cout << b0.x << " " << b0.y << " " << b0.z << " " << sum(b0) << std::endl;
//		std::cout << b1.x << " " << b1.y << " " << b1.z << " " << sum(b1) << std::endl;
//	}
//
//
//	bool line_tests() {
//		_screen.clear_init();
//
//		draw_line(Vec4(20, 30, 0, 1),	Vec4(400, 500, 0, 1), colors.red, _screen);
//		draw_line(Vec4(0, 0, 0, 1),		Vec4(1919, 1079, 0, 1), colors.red, _screen);
//		draw_line(Vec4(400, 300, 0, 1), Vec4(200, 500, 0, 1), colors.green, _screen);
//		draw_line(Vec4(300, 200, 0, 1), Vec4(-500, -400, 0, 1), colors.white, _screen);
//		draw_line(Vec4(400, 400, 0, 1), Vec4(0, 400, 0, 1), colors.blue, _screen);
//
//		bool result = _screen.save("tests/line_tests.tga");
//		_screen.clear_init();
//		return result;
//	}
//
//
//	bool triangle_tests() {
//		//_screen.clear_init();
//
//		//auto texture = TGAImage();
//		//texture.read_tga_file("obj/african_head/african_head_eye_outer_nm_tangent.tga");
//
//		//auto clamp = Vec2(_screen.get_width() - 1, _screen.get_height() - 1);
//
//		//Vec2 textures[3]	= { Vec2(0, 0), Vec2(255, 0), Vec2(255, 255) };
//		//Vec4 normals[3]		= { Vec4(0, 0, 1, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, 1, 0) };
//
//		//Vec4 tri_pts_array[3][3] = {
//		//	{ Vec4(400, 600, 0, 1), Vec4(300, 700, 0, 1),  Vec4(800, 850, 0, 1)  },
//		//	{ Vec4(100, 300, 0, 1), Vec4(200, 400, 0, 1),  Vec4(500, 550, 0, 1)  },
//		//	{ Vec4(900, 300, 0, 1), Vec4(1400, 400, 0, 1), Vec4(1700, 900, 0, 1) },
//		//};
//
//
//
//		//for (auto& tri_pts : tri_pts_array) {
//		//	Vec2 box[2];
//		//	Vec2 pts_2d[3] = {tri_pts[0], tri_pts[1], tri_pts[2]};
//		//	find_bounding_box(pts_2d, box, clamp);
//		//	draw_bounding_box(box, colors.red, _screen);
//		//	draw_triangle_outline(pts_2d, colors.magenta, _screen);
//		//	draw_triangle(tri_pts, , _screen, texture);
//		//}
//
//		//bool result = _screen.save("tests/triangle_tests.tga");
//
//		//_screen.clear_init();
//
//		//return result;
//	}
//
//
//	bool full_render_tests() {
//		_screen.clear_init();
//
//		Model model("obj/african_head/african_head.obj");
//
//		render_object_depr(model, _screen);
//		bool result1 = _screen.save("tests/poly_render_tests_old.tga");
//
//		_screen.clear_init();
//
//		//render_object_wire_depr(model, colors.cyan, _screen);
//		//bool result2 = _screen.save("tests/wire_render_tests.tga");
//
//		//_screen.clear_init();
//
//		return result1;
//	}
//};
//
//
