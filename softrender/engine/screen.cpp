#include <cassert>
#include <iostream>
#include <string>
#include <softrender/engine/screen.h>
#include <softrender/utils/tgaimage.h>

inline uint32_t pack_color(TGAColor& color, const bool ignore_alpha = true) {
	if (ignore_alpha) { color[3] = 255; }
	return (color[3] << 24) + (color[0] << 16) + (color[1] << 8) + color[2];
}
inline TGAColor unpack_color(uint32_t& color) {
	return {uint8_t(color & 255), 
			uint8_t(color >> 8 & 255), 
			uint8_t(color >> 16 & 255), 
			uint8_t(color >> 24 & 255)};
}

Screen::Screen(size_t width,
               size_t height,
               SDL_Texture& framebuffer,
               SDL_Renderer& renderer, 
               double zbuffer_depth,
               int xorientation,
               int yorientation,
               int zorientation,
               double camera_distance)
			:
			width(width),
			height(height),
			renderer_frame_buffer(framebuffer),
			renderer(renderer),
			zbuffer_depth(zbuffer_depth),
			Xorientation(xorientation),
			Yorientation(yorientation),
			Zorientation(zorientation),
			camera_dist_(camera_distance) {
	assert(width * height > 0);
	auto framebuffer_size = width * height;

	zbuffer = new double[framebuffer_size]{};
	for (size_t i = 0; i < framebuffer_size; i++) { zbuffer[i] = -std::numeric_limits<double>::max(); }

	local_frame_buffer = new uint32_t[framebuffer_size]{};

	SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 0);
}


Screen::~Screen() {
	delete[] zbuffer;
	delete[] local_frame_buffer;
	SDL_DestroyTexture(&renderer_frame_buffer);
	SDL_DestroyRenderer(&renderer);
}


const size_t& Screen::get_width() const							{ return width; }
const size_t& Screen::get_height() const						{ return height; }
const double& Screen::get_camera_distance() const				{ return camera_dist_; }
const int& Screen::get_camera_orientation(Axis3D axis) const	{ return axis ? (axis == 1 ? Yorientation : Zorientation) : Xorientation; }
const double& Screen::get_zbuffer_depth() const					{ return zbuffer_depth; }


void Screen::render_frame() {
    SDL_UpdateTexture(&renderer_frame_buffer, NULL, local_frame_buffer, 4 * width);
    SDL_RenderClear(&renderer);
    SDL_RenderCopy(&renderer, &renderer_frame_buffer, NULL, NULL);
    SDL_RenderPresent(&renderer);
}


bool Screen::zbuffer_test(const int x_screen, const int y_screen, const double Z) {
	bool result = false;
	static auto counter = 0;
	int idx = x_screen + y_screen * width;
	auto Zb = zbuffer[idx];
	if (Zorientation * Zb > Zorientation * Z) {
		result = true;
		counter++;
	}

	return result;
}


void Screen::zbuffer_set(int x_screen, int y_screen, double Z) const {
	zbuffer[x_screen + y_screen * width] = Z;
}


void Screen::clear_init() {
	for (size_t i = 0; i < width * height; i++) { local_frame_buffer[i] = 0; }
	for (size_t i = 0; i < width * height; i++) { zbuffer[i] = Zorientation * std::numeric_limits<double>::max(); }
}


void Screen::set(size_t x, size_t y, TGAColor& color) {
	assert(x < get_width() && y < get_height());
	uint32_t c = pack_color(color);
	local_frame_buffer[x + y * get_width()] = c;
}


bool Screen::save(const std::string& filename) const {
	auto w = get_width();
	auto h = get_height();
	auto img = TGAImage(w, h, 4);
	for (size_t idx = 0; idx < w * h; idx++) {
		uint32_t color = local_frame_buffer[idx]; // abgr -> bgra
		auto x = idx % w;
		auto y = idx / h;
		img.set(x, y, unpack_color(color));
	}
	return img.write_tga_file(filename);
}


void Screen::zbuffer_save(const std::string& filename) const {
	auto zmin = zbuffer_depth, zmax = -zbuffer_depth, zavg = 0.0;
	for (int i = 0; i < get_width() * height; i++) {
		if (auto depth = Zorientation * zbuffer[i]; 
			depth >= 0.0 && depth <= zbuffer_depth) 
		{
			if (zmin > zbuffer[i]) zmin = zbuffer[i];
			if (zmax < zbuffer[i]) zmax = zbuffer[i];
			zavg = (i * zavg + zbuffer[i]) / (i + 1);
		}
	}

	auto depth_map = TGAImage(get_width(), height, TGAImage::RGB);
	for (size_t i = 0; i < get_width() * height; i++) {
		auto x = i % get_width();
		auto y = i / get_width();
		auto relative_distance = (zbuffer[i] - zmin) / (zmax - zmin); // [0, 1]
		uint8_t distance = 255 * (Zorientation < 0 ? relative_distance : 1 - relative_distance);
		depth_map.set(x, y, TGAColor(distance, distance, distance));
	}

	auto result = depth_map.write_tga_file(filename);

	std::cerr << "min buffer z-depth: " << zmin << std::endl;
	std::cerr << "max buffer z-depth: " << zmax << std::endl;
	std::cerr << "avg buffer z-depth: " << zavg << std::endl << std::endl;

}
