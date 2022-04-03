#include <softrender/graphics/core.h>
#include <softrender/graphics/geometry.h>
#include <softrender/utils/tgaimage.h>
#include <omp.h>

//////////////////////
// ::Conventions:: //
////////////////////
// x+ => Right
// y+ => Up
// z+ => From display to viewer   
// Right-Hand Rule is being used
////////////////////////////////


Mat4 get_Rotation(Axis3D axis, double radians) {
	Mat4 rot = Mat4::identity();
	switch (axis) {
		case 0:
			rot[1][1] =  cos(radians); rot[1][2] = -sin(radians);
			rot[2][1] =  sin(radians); rot[2][2] =  cos(radians);
			break;
		case 1:
			rot[0][0] =  cos(radians); rot[0][2] =  sin(radians);
			rot[2][0] = -sin(radians); rot[2][2] =  cos(radians);
			break;
		case 2:
			rot[0][0] =  cos(radians); rot[0][1] = -sin(radians);
			rot[1][0] =  sin(radians); rot[1][1] =  cos(radians);
			break;
	}
	return rot;
}

Mat4 get_Projection(const Screen& screen) {
	auto D = screen.get_camera_distance();
	auto zdir = screen.get_camera_orientation(Axis3D::Z);
    return {{
	    {1, 0,		0, 0},
	    {0, 1,		0, 0},
	    {0, 0,		1, 0},
	    {0, 0, zdir/D, 1},
    }};
}


Mat4 get_Viewport(const Screen& screen, const Vec2& offset, double scale) {
	auto ar = static_cast<double>(screen.get_width()) / screen.get_height();
	auto w_scale = 0.5 * screen.get_width();
	auto h_scale = 0.5 * screen.get_height();
	auto d_scale = 0.5 * screen.get_zbuffer_depth();
	auto Xor = screen.get_camera_orientation(Axis3D::X);
	auto Yor = screen.get_camera_orientation(Axis3D::Y);
	auto Zor = screen.get_camera_orientation(Axis3D::Z);
	return {{
		{scale * w_scale,					 0.,	  0., offset.x + Xor * w_scale},
		{			  0.,  scale * ar * h_scale,	  0., offset.y + Yor * h_scale},
		{			  0.,					 0., d_scale,			 Zor * d_scale},
		{			  0.,					 0., 	  0.,						1.},
	}};
}


Mat4 get_View(const Vec3& view_point, const Vec3& target, const Vec3& up) {
    Vec3 z = (view_point - target).normalize();
    Vec3 x = (up ^ z).normalize();
    Vec3 y = (z ^ x).normalize();
	Mat4 Transition = {{ Vec4(x, 0), Vec4(y, 0), Vec4(z, 0), {0, 0, 0, 1} }};
    return Transition * Mat4::translation(-view_point);
}


void draw_line(Vec2 v0, Vec2 v1, TGAColor& color, Screen& screen) {
	bool is_steep = false;

	if (std::abs(v1.x - v0.x) < std::abs(v1.y - v0.y)) {
		std::swap(v0.x, v0.y);
		std::swap(v1.x, v1.y);
		is_steep = true;
	}

	if (v0.x > v1.x) {
		std::swap(v0, v1);
	}

	int dy = v1.y - v0.y;
	int dx = v1.x - v0.x;
	int dx_2 = dx * 2;
	int sign_dy = (dy > 0 ? 1 : -1);

	int y = v0.y;
	int error = 0;
	int derror = std::abs(dy) * 2;

	for (int x = v0.x; x <= v1.x; x++) {
		if (is_steep) {
			screen.set(y, x, color);
		} else {
			screen.set(x, y, color);
		}

		error += derror;
		if (error > dx) {
			error -= dx_2;
			y += sign_dy;
		}
	}
}


void draw_triangle_oldschool(Vec2 pts[3], TGAColor& color, Screen& screen) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; j++) pts[i][j] = static_cast<int>(pts[i][j]);

	if (int(pts[0].y) == int(pts[1].y) && int(pts[1].y) == int(pts[2].y)) return;

	if (pts[0].y > pts[1].y) { std::swap(pts[0], pts[1]); }
	if (pts[1].y > pts[2].y) { std::swap(pts[1], pts[2]); }
	if (pts[0].y > pts[1].y) { std::swap(pts[0], pts[1]); }

	auto total_height = pts[2].y - pts[0].y;
	for (int y = pts[0].y; y <= int(pts[2].y); y++) {
		bool second_half = y > pts[1].y;
		if (second_half) { std::swap(pts[0], pts[2]); }
		auto t = second_half ? pts[0].y - y : y - pts[0].y;
		auto segment_height = second_half ? pts[0].y - pts[1].y : pts[1].y - pts[0].y;
		auto x0 = pts[0].x + (pts[1].x - pts[0].x) * t / segment_height;
		auto x1 = pts[0].x + (pts[2].x - pts[0].x) * t / total_height;
		if (x0 > x1) { std::swap(x0, x1); }
		for (int x = x0; x <= x1; x++) {
			screen.set(x, y, color);
		}
	}
}


void find_bounding_box(const Vec2 pts[2], Vec2 bbox_out[2], const Vec2& clamp) {
	bbox_out[0][0] = bbox_out[0][1] =  std::numeric_limits<double>::max();
	bbox_out[1][0] = bbox_out[1][1] = -std::numeric_limits<double>::max();
	for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
			bbox_out[0][j] = std::max(0.0,      std::min(bbox_out[0][j], pts[i][j]));
            bbox_out[1][j] = std::min(clamp[j], std::max(bbox_out[1][j], pts[i][j]));
		}
	}
}


void draw_bounding_box(const Vec2 box[2], TGAColor& color, Screen& screen) {
	draw_line(box[0], Vec4(box[0].x, box[1].y, 0.0, 1.0), color, screen);
	draw_line(box[0], Vec4(box[1].x, box[0].y, 0.0, 1.0), color, screen);
	draw_line(Vec4(box[0].x, box[1].y, 0.0, 1.0), box[1], color, screen);
	draw_line(Vec4(box[1].x, box[0].y, 0.0, 1.0), box[1], color, screen);
}


Vec3 barycentric(const Vec2 pts[3], const Vec2& P) {
	auto AC_AB_PA_x = Vec3(pts[2].x-pts[0].x, pts[1].x-pts[0].x, pts[0].x-P.x);
	auto AC_AB_PA_y = Vec3(pts[2].y-pts[0].y, pts[1].y-pts[0].y, pts[0].y-P.y);
	auto cross = AC_AB_PA_x ^ AC_AB_PA_y;
	if (std::abs(cross.z) < 1) return {-1.0, -1.0, -1.0};
	return {
		1.0 - (cross.x / cross.z + cross.y / cross.z),
		cross.y / cross.z,
		cross.x / cross.z
	};
}

// TODO: fix & understand
Vec3 barycentric0(const Vec2 pts[3], const Vec2& P) {
	Mat3 ABC = {{ Vec3(pts[0]), Vec3(pts[1]), Vec3(pts[2]) }};
	if (auto det = ABC.det(); det < 1e-3) { return {-1.0, -1.0, -1.0}; }
	auto result = ABC.transpose().inverse() * Vec3(P);
	return result;
}


void viewport_orthogonal_depr(Vec3 vertices[3], const int count, const Screen& screen) {
	for (int i = 0; i < count; i++) {
		if (vertices[i].z > 0) return; 

		auto aspect_ratio = static_cast<double>(screen.get_height()) / screen.get_width();
		vertices[i].x = (0.5 * screen.get_width())  * (1.0 + vertices[i].x * aspect_ratio);
		vertices[i].y = (0.5 * screen.get_height()) * (1.0 + vertices[i].y);
	}
}


void draw_triangle(const Mat<3, 4>& pts_view, const Mat4& Viewport, IShader& shader, Screen& screen) {
	Vec4 pts_clip[3] =	{ pts_view[0]/pts_view[0].w, pts_view[1]/pts_view[1].w, pts_view[2]/pts_view[2].w };
	Vec2 pts_screen[3] = { Vec2(Viewport * pts_clip[0]), Vec2(Viewport * pts_clip[1]), Vec2(Viewport * pts_clip[2])};

	Vec2 P_screen, bbox_screen[2];
	Vec2 clamp(screen.get_width() - 1.0, screen.get_height() - 1.0);
	find_bounding_box(pts_screen, bbox_screen, clamp);

	for (P_screen.x = static_cast<int>(bbox_screen[0].x); P_screen.x <= static_cast<int>(bbox_screen[1].x); P_screen.x++) {
		for (P_screen.y = static_cast<int>(bbox_screen[0].y); P_screen.y <= static_cast<int>(bbox_screen[1].y); P_screen.y++) {
			auto bcc_screen = barycentric(pts_screen, P_screen);
			if (bcc_screen.z < 0 || bcc_screen.x < 0 || bcc_screen.y < 0)	{ continue; }

			auto bcc_clip = Vec3(bcc_screen.x / pts_view[0].w, bcc_screen.y / pts_view[1].w, bcc_screen.z / pts_view[2].w);
			bcc_clip = bcc_clip / (bcc_clip[0] + bcc_clip[1] + bcc_clip[2]);

			auto Z = bcc_clip * Vec3(pts_view[0].z, pts_view[1].z, pts_view[2].z);
			if (!screen.zbuffer_test(P_screen.x, P_screen.y, Z))			{ continue; }

			TGAColor color;
			if (bool discard = shader.fragment(bcc_clip, color))			{ continue; }
			screen.zbuffer_set(P_screen.x, P_screen.y, Z);
			screen.set(P_screen.x, P_screen.y, color);
		}
	}
}


void draw_triangle_outline(const Vec2 pts[2], TGAColor& color, Screen& screen) {
	draw_line(pts[0], pts[1], color, screen);
	draw_line(pts[1], pts[2], color, screen);
	draw_line(pts[2], pts[0], color, screen);
}




