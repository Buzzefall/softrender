#include <softrender/engine/scene.h>
#include <softrender/engine/screen.h>
#include <softrender/graphics/core.h>
#include <softrender/utils/utils.h>

Vec3 default_viewpoint			= {1, 1, 2};
Vec3 default_target				= {0, 0, 0};
Vec3 default_camera_orientation = {0, 1, 0};
Vec3 default_light_dir			= {0, 0, 1};

Scene::Scene(const std::vector<std::string>& object_names) : light_direction(default_light_dir) {
	load_objects(object_names);
}


void Scene::load_objects(const std::vector<std::string>& obj_names) {
	for (auto& name : obj_names) {
		objects.push_back(new RotatingObject(name));
	}
}


// TODO: Scene::save(path)
bool Scene::save(const std::string& path) { return false; }


void Scene::update(high_resolution_clock::time_point time) {
	for (auto& obj : objects) {
		obj->update(time);
	}
}


void Scene::render(Screen& target) {
	auto Viewport = get_Viewport(target);
	auto Projection = get_Projection(target);
	auto View = get_View(default_viewpoint, default_target, default_camera_orientation);

	target.clear_init();
	for (auto& obj : objects) {
		auto Model = obj->Position * obj->Rotation;
		//GouraudShader shader { light_direction, obj->model, (View * Model), Projection, Viewport };
		GouraudWireShader shader { 3e-2, light_direction, obj->model, (View * Model), Projection, Viewport };
		for (uint32_t iface = 0; iface < obj->model.get_faces_count(); iface++) {
			Mat<3, 4> verts_screenspace;
			for (int ivert = 0; ivert < 3; ivert++) {
				verts_screenspace[ivert] = shader.vertex(iface, ivert);
			}

			draw_triangle(verts_screenspace, Viewport, shader, target);
		} 
	}
}
