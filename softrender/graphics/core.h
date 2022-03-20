#pragma once

#include <softrender/graphics/screen.h>
#include <softrender/graphics/enum.h>
#include <softrender/graphics/geometry.h>
#include <softrender/graphics/model.h>
#include <softrender/utils/tgaimage.h>

struct IShader {
	const Model& model;

	Mat4 ViewModel;				// model	  -> per-Object instance <- accessed from scene.render()
	Mat4 Projection;			// projection -> per-camera			 <- accessed from scene.render()
	Mat4 Viewport;				// viewport	  -> per engine			 <- accessed from engine.render()

	static TGAColor sample2D(const Vec2& uv, const TGAImage& img) {
        return img.get(uv.x * img.get_width(), uv.y * img.get_height());
    }

	IShader(const Model& obj_model, Mat4 viewmodel_m, Mat4 projection_m, Mat4 viewport_m):
			model(obj_model), ViewModel(viewmodel_m), Projection(projection_m), Viewport(viewport_m) { }

	IShader(const IShader& shader) = delete;
	IShader(const IShader&& shader) = delete;
	IShader& operator=(const IShader&) = delete;
	IShader& operator=(const IShader&&) = delete;

	virtual ~IShader() = default;
    virtual Vec4 vertex(uint32_t iface, uint32_t ivert) = 0;
	virtual bool fragment(Vec3 baryc, TGAColor& gl_FragColor) = 0;
};


struct GouraudShader : IShader {
	Vec3 light_direction = {};
	
	Mat<3,3> view_tri;		// per-vertex batch data
	Mat<2,3> varying_uv;	// per-vertex & per-fragment data
	Mat<3,3> varying_norms;	// per-vertex & per-fragment data

	GouraudShader(const Vec3& light_dir, const Model& obj_model, Mat4 viewmodel_m, Mat4 projection_m, Mat4 viewport_m);

	Vec4 vertex(uint32_t iface, uint32_t ivert) override;
	bool fragment(Vec3 baryc, TGAColor& gl_FragColor) override;
};

struct GouraudWireShader : GouraudShader {
	double thickness = 3e-2;

	GouraudWireShader(double thickness, const Vec3& light_dir, const Model& obj_model, Mat4 viewmodel_m, Mat4 projection_m, Mat4 viewport_m);
	bool fragment(Vec3 baryc, TGAColor& gl_FragColor) override;
};


Mat4 get_Rotation(Axis3D axis, double radians);
Mat4 get_View(const Vec3& view_point, const Vec3& target, const Vec3& up);
Mat4 get_Projection(const Screen& screen);
Mat4 get_Viewport(const Screen& screen, const Vec2& offset = {0.0, 0.0}, double scale = 1.0);

Vec3 barycentric0(const Vec2 pts[3], const Vec2& P);
Vec3 barycentric(const Vec2 pts[3], const Vec2& P);

double light_intensity(const Vec3& face_norm, const Vec3& light_dir);

void draw_line(Vec2 v0, Vec2 v1, TGAColor& color, Screen& screen);

void find_bounding_box(const Vec2 pts[2], Vec2 bbox_out[2], const Vec2& clamp);
void draw_bounding_box(const Vec2 bbox[2], TGAColor& color, Screen& screen);

void draw_triangle(const Mat<3, 4>& pts_view, const Mat4& Viewport, IShader& shader, Screen& screen);
void draw_triangle_oldschool(Vec2 pts[3], TGAColor& color, Screen& screen);
void draw_triangle_outline(const Vec2 pts[2], TGAColor& color, Screen& screen);

void viewport_orthogonal_depr(Vec3 vertices[3], const int count, const Screen& screen);

void render_object_wire_depr(Model& model, TGAColor& color, Screen& screen);
