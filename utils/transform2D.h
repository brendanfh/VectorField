#ifndef _TRANSFORM_2D_
#define _TRANSFORM_2D_

#include <memory>

class Transform2D {
	private:
		float tx, ty;
		float sx, sy;

	public:
		Transform2D();
		~Transform2D();

		auto translate(float dx, float dy) -> void;
		auto scale(float dx, float dy) -> void;

		auto to_matrix() const -> std::unique_ptr<float[]>;
};

#endif
