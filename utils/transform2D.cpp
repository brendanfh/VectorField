#include "./transform2D.h"

Transform2D::Transform2D()
	: tx(0.f), ty(0.f), sx(1.0f), sy(1.0f)
{
}


Transform2D::~Transform2D() {
}


auto Transform2D::translate(float dx, float dy) -> void {
	tx += dx;
	ty += dy;
}


auto Transform2D::scale(float dx, float dy) -> void {
	float ax = tx / sx;
	float ay = ty / sy;
	sx *= dx;
	sy *= dy;
	tx = ax * sx;
	ty = ay * sy;
}


auto Transform2D::to_matrix() const -> std::unique_ptr<float[]> {
	std::unique_ptr<float[]> ret(new float[9]);

	ret[0] = sx;  ret[1] = 0.0f; ret[2] = 0.0f;
	ret[3] = 0.f; ret[4] = sy;   ret[5] = 0.0f;
	ret[6] = tx;  ret[7] = ty;   ret[8] = 1.0f;

	return ret;
}
