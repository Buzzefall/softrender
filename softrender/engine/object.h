#pragma once

#include <softrender/graphics/model.h>
#include <softrender/utils/utils.h>

struct Object {
	Model model;

	Mat4 Position = Mat4::identity();
	Mat4 Rotation = Mat4::identity();

	explicit Object(const std::string& object_filename);
	virtual ~Object() = default;

	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;

	virtual void update(high_resolution_clock::time_point time);
};

struct RotatingObject final : Object {
	explicit RotatingObject(const std::string& object_filename);

	void update(high_resolution_clock::time_point time) override;
};

