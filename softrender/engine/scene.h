#pragma once

#include <string>
#include <vector>
#include <softrender/engine/object.h>
#include <softrender/engine/screen.h>
#include <softrender/utils/utils.h>

class Scene {
	Vec3 origin = {0, 0, 0};
	Vec3 light_direction = {0, 0, 1};
	std::vector<Object*> objects;

	//void save_frame(Screen& target, const std::string& filepath); TO DELETE

public:
	explicit Scene(const std::vector<std::string>& object_names);
	~Scene() {
		for (auto& object : objects) delete object;
	}

	[[nodiscard]] bool save(const std::string& path);
	void load_objects(const std::vector<std::string>& obj_names);
	void update(high_resolution_clock::time_point time);
	void render(Screen& target);
};
