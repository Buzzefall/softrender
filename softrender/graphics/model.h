#pragma once

#include <string>
#include <vector>
#include <softrender/graphics/geometry.h>
#include <softrender/utils/tgaimage.h>

class Model {
	std::vector<Vec4> vertices;
	std::vector<Vec2> texcoords;
	std::vector<Vec4> vertexnormals;

	std::vector<int> face_vertices;
	std::vector<int> face_texcoords;
	std::vector<int> face_normals;

	TGAImage diffusemap;
	TGAImage normalmap;
	TGAImage specmap;

	bool load_object(const std::string& object_name);
	static bool load_texture(const std::string& object_filename, const std::string& suffix, TGAImage& image);

public:
	explicit Model(const std::string& obj_filename);
	Model(const Model& original) = default;
	~Model() = default;

	[[nodiscard]] uint32_t get_verts_count() const;
	[[nodiscard]] uint32_t get_normals_count() const;
	[[nodiscard]] uint32_t get_texcoords_count() const;

	[[nodiscard]] uint32_t get_faces_count() const;

	[[nodiscard]] uint32_t get_diffusemap_size() const;
	[[nodiscard]] const TGAImage& get_diffusemap() const;

	[[nodiscard]] Vec4 get_vertex(uint64_t iface, uint64_t ivert) const;
	[[nodiscard]] Vec4 get_normal(uint64_t iface, uint64_t ivert) const;
	[[nodiscard]] Vec2 get_texcoords(uint64_t iface, uint64_t ivert) const;
};
