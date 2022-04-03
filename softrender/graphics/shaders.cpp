#include <iostream>
#include <softrender/graphics/shaders.h>


double light_intensity(const Vec3& face_norm, const Vec3& light_dir) {
	return light_dir * face_norm;
}


GouraudShader::GouraudShader(const Vec3& light_dir, const Model& obj_model, Mat4 viewmodel_m, Mat4 projection_m, Mat4 viewport_m)
	: IShader(obj_model, viewmodel_m, projection_m, viewport_m),
	  light_direction(light_dir) { }


Vec4 GouraudShader::vertex(uint32_t iface, uint32_t ivert) {
	varying_norms.set_col(Vec3(ViewModel.transpose().inverse() * model.get_normal(iface, ivert)).normalize(), ivert);
	varying_uv.set_col(model.get_texcoords(iface, ivert), ivert);
	auto gl_Position = ViewModel * model.get_vertex(iface, ivert);
	view_tri.set_col(gl_Position, ivert);
	gl_Position = Projection * gl_Position;
	return gl_Position;
}


bool GouraudShader::fragment(Vec3 baryc, TGAColor& gl_FragColor) {
	Vec3 n_frag = varying_norms * baryc;
	Vec2 uv_frag = varying_uv * baryc;
	TGAColor color = sample2D(uv_frag, model.get_diffusemap());

	bool discard = true;
	if (auto intensity = light_intensity(n_frag, light_direction); intensity >= 0.0) {
		gl_FragColor = color * intensity;
		return !discard;
	}
	return discard;
}


GouraudWireShader::GouraudWireShader(double thickness, const Vec3& light_dir, const Model& obj_model, Mat4 viewmodel_m, Mat4 projection_m, Mat4 viewport_m)
	: GouraudShader(light_dir, obj_model, viewmodel_m, projection_m, viewport_m),
	  thickness(thickness) { }


bool GouraudWireShader::fragment(Vec3 baryc, TGAColor& gl_FragColor) {
	auto discard = baryc.x < thickness || baryc.y < thickness || baryc.z < thickness;
	return discard && GouraudShader::fragment(baryc, gl_FragColor);
}


HeatMapShader::HeatMapShader(Vec3 heat_source, const Vec3& light_dir, const Model& obj_model, Mat4 viewmodel_m, Mat4 projection_m, Mat4 viewport_m)
	: GouraudShader(light_dir, obj_model, viewmodel_m, projection_m, viewport_m),
	  heat_source(heat_source) { }


bool HeatMapShader::fragment(Vec3 baryc, TGAColor& gl_FragColor) {
	bool discard = GouraudShader::fragment(baryc, gl_FragColor);

	if (discard) return discard;

	auto frag_point = view_tri * baryc;
	auto max_heat_distance = 0.25;
	auto min_heat_distance = 3.5;
	auto dist = (heat_source - frag_point).norm();
	auto heat = (min_heat_distance - dist) / (min_heat_distance - max_heat_distance);

	auto r_green = 0.75;
	auto l_green = 0.25;

	auto red_contrib	= exp(pow(heat - 1.0, 2));
	auto green_contrib	= exp(pow(heat - 0.5, 2));
	auto blue_contrib	= exp(pow(heat - 0.0, 2));							

	//auto red_contrib	= (heat > 0.5)							? 2.0 * (heat - 0.5) : 0;
	//auto green_contrib	= (heat >= l_green && heat <= r_green)	? 4.0 * (0.25 - abs(heat - 0.5)) : 0;
	//auto blue_contrib	= (heat < 0.5)							? 2.0 * abs(heat - 0.5) : 0;

	//std::cout << "Distance: " << dist << std::endl;
	//std::cout << "Heat: " << heat << std::endl;

	gl_FragColor = TGAColor(255*red_contrib, 255*green_contrib, 255*blue_contrib);

	return discard;
}
