#include <softrender/engine/object.h>
#include <softrender/graphics/core.h>
#include <softrender/utils/utils.h>

Object::Object(const std::string& object_filename) : model(object_filename) { }

void Object::update(high_resolution_clock::time_point time) { }


RotatingObject::RotatingObject(const std::string& object_filename): Object(object_filename) { }

void RotatingObject::update(high_resolution_clock::time_point time) {
	Object::update(time);

	static auto past = time;
	constexpr auto step = M_PI * 7e-5;
	auto radians = step * duration<double, milli>(time - past).count();
	//Rotation = get_Rotation(Axis3D::X, radians) * get_Rotation(Axis3D::Y, radians);
	Rotation = get_Rotation(Axis3D::Y, radians);
}
