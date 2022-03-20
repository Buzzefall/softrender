#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <softrender/graphics/model.h>

uint32_t Model::get_verts_count() const		{ return vertices.size();		}
uint32_t Model::get_texcoords_count() const	{ return texcoords.size();		}
uint32_t Model::get_normals_count() const	{ return vertexnormals.size(); }

uint32_t Model::get_faces_count() const		{ return face_vertices.size() / 3; }

uint32_t Model::get_diffusemap_size() const		{ return diffusemap.get_width(); }
const TGAImage& Model::get_diffusemap() const	{ return diffusemap;			  }

Vec4 Model::get_vertex(const uint64_t iface, const uint64_t ivert) const	  { return vertices[face_vertices[3ULL * iface + ivert]];		}
Vec4 Model::get_normal(const uint64_t iface, const uint64_t ivert) const	  { return vertexnormals[face_normals[3ULL * iface + ivert]]; }
Vec2 Model::get_texcoords(const uint64_t iface, const uint64_t ivert) const   { return texcoords[face_texcoords[3ULL * iface + ivert]];	}


bool Model::load_object(const std::string &object_name) {
	std::ifstream data_stream;
	data_stream.open(object_name, std::ifstream::in);
	if (data_stream.fail()) {
		throw std::runtime_error("Failed to open model file. Ensure it is in ./obj/object_name_passed/");
	}

	char trash;
	std::string line;
	while(!data_stream.eof()) {
		std::getline(data_stream, line);
		std::istringstream iss(line);

		if (line.compare(0, 2, "v ") == 0) {
			float coords[3];
			iss >> trash;
			iss >> coords[0] >> coords[1] >> coords[2];
			vertices.emplace_back(coords[0], coords[1], coords[2], 1.0);
		}  else if (line.compare(0, 3, "vt ") == 0) {
			float coords[3];
			iss >> trash >> trash;
			iss >> coords[0] >> coords[1] >> coords[2];
			texcoords.emplace_back(coords[0], coords[1]);
		} else if (line.compare(0, 3, "vn ") == 0) {
			float coords[3];
			iss >> trash >> trash;
			iss >> coords[0] >> coords[1] >> coords[2];
			vertexnormals.emplace_back(coords[0], coords[1], coords[2], 0.0);
		} else if (line.compare(0, 2, "f ") == 0) {
			std::vector<int> vertex_indices, texture_indices, normal_indices;
			int ivert, itex, inorm;
			iss >> trash;
			while (iss >> ivert >> trash >> itex >> trash >> inorm) {
				face_vertices.push_back(--ivert);
				face_texcoords.push_back(--itex);
				face_normals.push_back(--inorm);
			}
		}
	}

	data_stream.close();
	
	std::cout << "Object loaded from " << object_name << ".\n";

	return true;
}


bool Model::load_texture(const std::string& object_filename, const std::string& suffix, TGAImage& image) {
	auto path = object_filename.substr(0, object_filename.find_last_of('.')) + suffix;
	auto success = image.read_tga_file(path);
	if (success) {
		image.flip_vertically();
		std::cout << "Texture loaded from " << path << ": ";
		std::cout << image.get_width() << "x" << image.get_height() << "/" << 8 * image.get_bytespp() << ".\n";
	}
	return success;
}


Model::Model(const std::string& obj_filename) {
	load_object(obj_filename);
	load_texture(obj_filename, "_diffuse.tga", diffusemap);
	load_texture(obj_filename, "_nm_tangent.tga", normalmap);
	load_texture(obj_filename, "_spec.tga", specmap);

#ifdef __DEBUG__
	std::cerr << "\nModel loaded. Statistics: "									<< std::endl;
	std::cout << "---------------------------"									<< std::endl;
	std::cout << "vertices # "						<< vertices.size()			<< std::endl;
	std::cout << "texcoords # "						<< texcoords.size()			<< std::endl;
	std::cout << "normals # "						<< vertexnormals.size()		<< std::endl;
	std::cout << "---------------------------"									<< std::endl;
	std::cout << "Face vertex indices # "			<< face_vertices.size()		<< std::endl;
	std::cout << "Face texcoords indices # "		<< face_texcoords.size()	<< std::endl;
	std::cout << "Face vertex normals indices # "	<< face_normals.size()		<< std::endl;
	std::cout << "---------------------------"									<< std::endl << std::endl;
#endif

	assert(face_vertices.size() == face_normals.size() && face_vertices.size() == face_texcoords.size());
}

