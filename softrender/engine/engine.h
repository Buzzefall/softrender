#pragma once

#include <SDL.h>
#include <softrender/engine/scene.h>
#include <softrender/graphics/screen.h>
#include <softrender/utils/utils.h>

constexpr uint16_t FPS_LIMITER_60 = 16;

class Engine
{
	Scene* scene = nullptr;
	Screen* render_target = nullptr;
	high_resolution_clock::time_point time = high_resolution_clock::now();

	bool shutdown = false;

public:
	Engine() = default;
	~Engine() { delete scene; }


	[[nodiscard]] static bool save_frame(Screen& target, const std::string& filepath);
	[[nodiscard]] static bool save_scene(Scene& scene, const std::string& path);

	void load_scene(const std::vector<std::string>& object_names);
	void set_render_target(size_t screen_width, size_t screen_height, SDL_Renderer& renderer, SDL_Texture& framebuffer);
	bool validate_initialization();

	void run();
	void update_world();
	void render_frame();
};

