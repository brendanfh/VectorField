#ifndef _BODY_
#define _BODY_

#include "../../utils/vector2D.h"

class Body {
    private:
        Vector2D body_pos;
        Vector2D body_vel;
        Vector2D body_acc;
        float mass;
    public:
        Body(Vector2D&& start_pos, float start_mass);

        auto get_position() -> Vector2D&;
        auto get_mass() -> float;
        auto get_force_position(int scale) -> Vector2D&;
        auto apply_force(Vector2D&& force) -> void;

        ~Body();
};

#endif