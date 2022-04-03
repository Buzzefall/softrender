#pragma once

#include <SDL_render.h>
#include <string>
#include <softrender/graphics/enum.h>
#include <softrender/utils/tgaimage.h>

class Screen {
	size_t width;
	size_t height;

	SDL_Texture& renderer_frame_buffer;
	SDL_Renderer& renderer;

	uint32_t* local_frame_buffer;		
	double* zbuffer;
	double zbuffer_depth;

	int Xorientation;
	int Yorientation;
	int Zorientation;
	double camera_dist_;

public:
	Screen(size_t width,
	       size_t height,
		   SDL_Texture& framebuffer,
           SDL_Renderer& renderer, 
	       double zbuffer_depth	  = 255.0, 
	       int xorientation		  =  1,
	       int yorientation		  =  1,
	       int zorientation		  = -1,
	       double camera_distance = 1.0);

	~Screen();



	[[nodiscard]] const size_t& get_width() const;
	[[nodiscard]] const size_t& get_height() const;
	[[nodiscard]] const double& get_camera_distance() const;
	[[nodiscard]] const int& get_camera_orientation(Axis3D axis) const;
	[[nodiscard]] const double& get_zbuffer_depth() const;

	
	void clear_init();
	void render_frame() const;
	void zbuffer_set(int x_screen, int y_screen, double Z) const;
	void zbuffer_save(const std::string& filename = "tests/zbuffer.tga") const;
	[[nodiscard]] bool zbuffer_test(int x_screen, int y_screen, double Z);


	void set(size_t x, size_t y, TGAColor& color);
	[[nodiscard]] bool save(const std::string& filename) const;

};
