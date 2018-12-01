#ifndef _BODY_
#define _BODY_

#include "../../utils/vector2D.h"

class Body {
    private:
        Vector2D body_pos;
        Vector2D body_vel;
        Vector2D body_acc;
        float mass;

        int time = 300;

    public:
        Body();
        Body(Vector2D&& start_pos, float start_mass);

        auto update(float t) -> void;
        auto get_position() -> Vector2D&;
        auto set_pos(float x, float y) -> void;
        auto get_mass() -> float;
        auto get_force_position(int scale) -> Vector2D;
        auto apply_force(Vector2D&& force) -> void;

        ~Body();
};

#endif