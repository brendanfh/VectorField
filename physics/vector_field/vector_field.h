#ifndef _VECTOR_FIELD_
#define _VECTOR_FIELD_

#include "../../utils/vector2D.h"
#include <cstdlib>
#include <memory>
#include <iostream>

class Vector_Field {
    private:
        int field_height;
        int field_width;
        std::unique_ptr<Vector2D[]> field;
        std::unique_ptr<Vector2D[]> newField;
        int field_scale_factor;

        auto is_part_of_field(int width, int height) -> bool;
       
    public:
        Vector_Field(int scale_factor, int width, int height);

        auto step() -> void;
        auto add_gravity_well(int x, int y, float gravForce) -> void;
        auto get_force(int x, int y) -> Vector2D;
        auto get_scale_factor() -> int;

        ~Vector_Field();
};

#endif