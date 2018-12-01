#include "body.h"
#include <cmath>

Body::Body(Vector2D&& start_pos, float start_mass) {
    this->body_pos = start_pos;
    this->mass = start_mass;

    this->body_vel = 0;
    this->body_acc = 0;
}


auto Body::get_position() -> Vector2D& {
    return this->body_pos;
}


auto Body::get_mass() -> float {
    return this->mass;
}


auto Body::get_force_position(int scale) -> Vector2D {
    Vector2D pos = this->body_pos / scale;
    pos.map([](float f) { return std::floor(f); });
    return pos;
}


auto Body::apply_force(Vector2D&& force) -> void {
    this->body_acc = force/this->mass;
    this->body_vel += this->body_acc;
    this->body_pos += this->body_vel;
}


Body::~Body() {
    
}