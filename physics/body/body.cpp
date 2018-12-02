#include "body.h"
#include <cmath>

Body::Body()
    : body_pos(Vector2D(0, 0)), body_vel(Vector2D(0, 0)), body_acc(Vector2D(0, 0)), mass(rand() % 10 + 3)
{
}

Body::Body(Vector2D&& start_pos, float start_mass) {
    this->body_pos = start_pos;
    this->mass = start_mass;

    this->body_vel = 0;
    this->body_acc = 0;
}


auto Body::get_position() -> Vector2D& {
    return this->body_pos;
}

auto Body::set_pos(float x, float y) -> void {
    this->body_pos.x = x;
    this->body_pos.y = y;
}

auto Body::get_mass() -> float {
    return this->mass;
}


auto Body::get_force_position(int scale) -> Vector2D {
    Vector2D pos = this->body_pos / scale;
    pos.map([](float f) { return std::floor(f); });
    return pos;
}

auto Body::update(float t) -> void {
    this->body_acc *= .80;
    this->body_vel = this->body_vel + (this->body_acc * t);
    this->body_vel *= .95;
    this->body_pos = this->body_pos + (this->body_vel * t);

    this->time--;
}

auto Body::apply_force(Vector2D&& force) -> void {
    this->body_acc = this->body_acc + (force/this->mass);
}


Body::~Body() {
    
}