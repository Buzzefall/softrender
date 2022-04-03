#pragma once

#include <softrender/graphics/model.h>
#include <softrender/utils/tgaimage.h>


class IShader {
public:

	IShader(const Model& obj_model, Mat4 viewmodel_m, Mat4 projection_m, Mat4 viewport_m):
			model(obj_model), ViewModel(viewmodel_m), Projection(projection_m), Viewport(viewport_m) { }

	IShader(const IShader& shader) = delete;
	IShader(const IShader&& shader) = delete;
	IShader& operator=(const IShader&) = delete;
	IShader& operator=(const IShader&&) = delete;

	virtual ~IShader() = default;
    virtual Vec4 vertex(uint32_t iface, uint32_t ivert) = 0;
	virtual bool fragment(Vec3 baryc, TGAColor& gl_FragColor) = 0;

protected:
	const Model& model;

	Mat4 ViewModel;				// model	  -> per-Object instance <- accessed from scene.render()
	Mat4 Projection;			// projection -> per-camera			 <- accessed from scene.render()
	Mat4 Viewport;				// viewport	  -> per engine			 <- accessed from engine.render()

	static TGAColor sample2D(const Vec2& uv, const TGAImage& img) {
        return img.get(uv.x * img.get_width(), uv.y * img.get_height());
    }
};


class GouraudShader : public IShader {
public:
	GouraudShader(const Vec3& light_dir, const Model& obj_model, Mat4 viewmodel_m, Mat4 projection_m, Mat4 viewport_m);

	Vec4 vertex(uint32_t iface, uint32_t ivert) override;
	bool fragment(Vec3 baryc, TGAColor& gl_FragColor) override;

protected:
	Vec3 light_direction = {};

	Mat<3,3> view_tri;		// per-vertex batch data
	Mat<2,3> varying_uv;	// per-vertex & per-fragment data
	Mat<3,3> varying_norms;	// per-vertex & per-fragment data
};


class GouraudWireShader : public GouraudShader {
public:
	GouraudWireShader(double thickness, const Vec3& light_dir, const Model& obj_model, Mat4 viewmodel_m, Mat4 projection_m, Mat4 viewport_m);
	bool fragment(Vec3 baryc, TGAColor& gl_FragColor) override;

protected:
	double thickness = 3e-2;
};


class HeatMapShader : public GouraudShader {
public:
	HeatMapShader(Vec3 heat_source, const Vec3& light_dir, const Model& obj_model, Mat4 viewmodel_m, Mat4 projection_m, Mat4 viewport_m);
	bool fragment(Vec3 baryc, TGAColor& gl_FragColor) override;

protected:
	Vec3 heat_source = {};
};