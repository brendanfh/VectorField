#include "vector_field.h"

Vector_Field::Vector_Field(int scale_factor, int width, int height) {
    field_height = height;
    field_width = width;
    field_scale_factor = scale_factor;

    field = std::unique_ptr<Vector2D[]>(new Vector2D[width * height]);
    newField = std::unique_ptr<Vector2D[]>(new Vector2D[width * height]);
}

auto Vector_Field::is_part_of_field(int width, int height) -> bool {
    if(width < 0 || width > field_width)
        return false;
    else if(height < 0 || height > field_height)
        return false;

    return true;
}

auto Vector_Field::step() -> void {
    for(int current_width_pos = 0; current_width_pos < this->field_width; current_width_pos++) {
        for(int current_height_pos = 0; current_height_pos < this->field_height; current_height_pos++) {
            float cum_x = 0, cum_y = 0, cum_mag;                        
            for(int i = -1; i <= 1; i++) {
                int width_to_test = current_width_pos + i;
                for(int x = -1; x <= 1; x++) {
                    int height_to_test = current_width_pos + x;           
                    if(is_part_of_field(width_to_test, height_to_test)) {
                        Vector2D vector_to_test = field[height_to_test * field_width + width_to_test];
                        cum_x += vector_to_test.x * vector_to_test.magnitude();
                        cum_y += vector_to_test.y * vector_to_test.magnitude();
                        cum_mag += vector_to_test.magnitude();
                    }
                }
            }
            newField[current_height_pos * field_width + current_width_pos] = Vector2D(cum_x/cum_mag,cum_y/cum_mag);                                                                                                                                                            
        }
    }
    newField.swap(field);
}


auto Vector_Field::add_gravity_well(int x, int y, float gravForce) -> void {
    if(!is_part_of_field(x, y)) {
        std::cout << "bro for real" << std::endl;
        return;        
    }
       
    for(int w = -1; w <= 1; w++) {
        int width_to_test = x + w;
        for(int h = -1; h <= 1; h++) {
            int height_to_test = y + h;           
            if(is_part_of_field(width_to_test, height_to_test)) {
                if(w == x && h == y)
                    continue;
                else {
                    Vector2D& adjacent = field[width_to_test + (field_width * height_to_test)];
                    adjacent.x = w * -1;
                    adjacent.y = h * -1;
                    adjacent.normalized();
                    adjacent *= gravForce;
                }
            }
            else {
                std::cout << "really?" << std::endl;
            }
        }
    }    
}


auto Vector_Field::get_force(int x, int y) -> Vector2D {
    if(is_part_of_field(x,y))
        return field[x + (field_width * y)];
    return Vector2D(0,0);
}

auto Vector_Field::get_scale_factor() -> int {
    return field_scale_factor;        
}


Vector_Field::~Vector_Field() {
    field_height = 0;
    field_width = 0;
}
