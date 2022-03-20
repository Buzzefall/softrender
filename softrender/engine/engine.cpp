#include <iostream>
#include <stdexcept>
#include <thread>
#include <softrender/engine/engine.h>


void Engine::load_scene(const std::vector<std::string>& object_names) {
	if (object_names.empty()) { throw std::invalid_argument("Path to scene data is empty or wrong."); }
	scene = new Scene(object_names);
}


bool Engine::save_scene(Scene& scene, const std::string& path) {
	return scene.save(path);
}


bool Engine::save_frame(Screen& target, const std::string& filepath) {
	return target.save(filepath);
}


void Engine::run() {
	if (!validate_initialization()) { throw std::runtime_error("Attempting to run Engine when initialization is not complete."); }

	SDL_Event event;
	auto t0 = high_resolution_clock::now();
	auto past = t0;
	while(!shutdown) {
		if (SDL_PollEvent(&event)) {
	        if (event.type == SDL_QUIT ||
		        event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		        shutdown = true;
	        }
        }

		update_world();

		auto now = high_resolution_clock::now();
		duration<double, milli> delta = now - past;
		past = now;

		if (delta.count() < FPS_LIMITER_60) { std::this_thread::sleep_for(delta); }

		render_frame();

		std::cout << "Frame rate: " << 1000 * pow(delta.count(), -1) << std::endl;
	}
}


void Engine::set_render_target(size_t screen_width, size_t screen_height, SDL_Renderer& renderer, SDL_Texture& framebuffer) {
	delete render_target;
	render_target = new Screen(screen_width, screen_height, framebuffer, renderer);
}


bool Engine::validate_initialization() {
	if (shutdown || scene == nullptr || render_target == nullptr) { return false; }
	return true;
}


void Engine::update_world() {
	time = high_resolution_clock::now();
	scene->update(time);
}

void Engine::render_frame() {
	scene->render(*render_target);
	render_target->render_frame();
}