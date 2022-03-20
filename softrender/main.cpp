#include <iostream>
#include <SDL.h>
#include <softrender/engine/engine.h>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

// Entry Point
int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Provide at least 1 path to object: <obj_dir>/<obj_name>.obj" << std::endl;
		return EXIT_FAILURE;
	}

	if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

	std::vector<std::string> object_names{};

	for (int i = 1; i < argc; i++) {
		object_names.emplace_back(argv[i]);
		std::cout << object_names[i-1] << std::endl;
	}

	auto engine = Engine();
	engine.load_scene(object_names);


	SDL_Window   *window   = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (0 > SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer)) {
	    std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
	    return EXIT_FAILURE;
	}

	SDL_Texture *framebuffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (nullptr == framebuffer_texture) {
        std::cerr << "Failed to create framebuffer texture: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

	engine.set_render_target(SCREEN_WIDTH, SCREEN_HEIGHT, *renderer, *framebuffer_texture);
	engine.run();

	SDL_DestroyWindow(window);
    SDL_Quit();

	return EXIT_SUCCESS;
}

 
