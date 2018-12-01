#include "vector_field.h"

Vector_Field::Vector_Field(int scale_factor, int width, int height) {
    field_height = height;
    field_width = width;
    field_scale_factor = scale_factor;

    field = std::unique_ptr<Vector2D[]>(new Vector2D[width * height]);
    for (int i = 0; i < width * height; i++) {
        field[i] = Vector2D(0, 0);
    }

    newField = std::unique_ptr<Vector2D[]>(new Vector2D[width * height]);
}

inline auto Vector_Field::is_part_of_field(int width, int height) -> bool {
    if(width < 0 || width >= field_width)
        return false;
    else if(height < 0 || height >= field_height)
        return false;

    return true;
}

auto Vector_Field::step() -> void {
    for(int current_width_pos = 0; current_width_pos < this->field_width; current_width_pos++) {
        for(int current_height_pos = 0; current_height_pos < this->field_height; current_height_pos++) {
            float cum_x = 0, cum_y = 0, cum_mag = 0, mag;
            for(int i = -1; i <= 1; i++) {
                int width_to_test = current_width_pos + i;
                for(int x = -1; x <= 1; x++) {
                    int height_to_test = current_height_pos + x;
                    if(is_part_of_field(width_to_test, height_to_test)) {
                        Vector2D vector_to_test = field[height_to_test * field_width + width_to_test];
                        mag = vector_to_test.magnitude();
                        cum_x += vector_to_test.x * mag;
                        cum_y += vector_to_test.y * mag;
                        cum_mag += mag;
                    }
                }
            }
            if (cum_mag != 0)
                newField[current_height_pos * field_width + current_width_pos] = (Vector2D(cum_x,cum_y) / cum_mag) * .999;
            else
                newField[current_height_pos * field_width + current_width_pos] = 0;
        }
    }
    newField.swap(field);
}

auto Vector_Field::step(int pos) -> void {
    int row = pos / field_width;
    int column = pos % field_width;

    float cum_x = 0, cum_y = 0, cum_mag, mag;
    for(int i = -1; i <= 1; i++) {
        int width_to_test = column + i;
        for(int x = -1; x <= 1; x++) {
            int height_to_test = row + x;
            if(is_part_of_field(width_to_test, height_to_test)) {
                Vector2D vector_to_test = field[height_to_test * field_width + width_to_test];
                mag = vector_to_test.magnitude();
                cum_x += vector_to_test.x * mag;
                cum_y += vector_to_test.y * mag;
                cum_mag += mag;
            }
        }
    }
    newField[row * field_width + column] = Vector2D(cum_x/cum_mag,cum_y/cum_mag);
    newField.swap(field);
}

auto Vector_Field::add_gravity_well(int x, int y, float gravForce) -> void {
    if(!is_part_of_field(x, y)) {
        std::cout << "Attempted to add gravity well ouside of field" << std::endl;
        return;        
    }
       
    int width_to_test, height_to_test, val;
    for(int w = -1; w <= 1; w++) {
        width_to_test = x + w;
        for(int h = -1; h <= 1; h++) {
            height_to_test = y + h;           
            if(is_part_of_field(width_to_test, height_to_test)) {
                val = width_to_test + (field_width * height_to_test);
                Vector2D adj(-w, -h);
                field[val] = adj.normalized();
                field[val] *= gravForce;
            }
        }
    }    
}

auto Vector_Field::add_explosion(int x, int y, float gravForce) -> void {
    if(!is_part_of_field(x, y)) {
        std::cout << "Attempted to add explosion ouside of field" << std::endl;
        return;        
    }
       
    for(int w = -1; w <= 1; w++) {
        int width_to_test = x + w;
        for(int h = -1; h <= 1; h++) {
            int height_to_test = y + h;           
            if(is_part_of_field(width_to_test, height_to_test)) {
                Vector2D& adjacent = field[width_to_test + (field_width * height_to_test)];
                adjacent.x = w;
                adjacent.y = h;
                adjacent = adjacent.normalized() * gravForce;
            }
        }
    }    
}


auto Vector_Field::add_curl(int x, int y, float gravForce) -> void {
    if(!is_part_of_field(x, y)) {
        std::cout << "Attempted to add curl ouside of field" << std::endl;
        return;        
    }

    Vector2D ourVector = field[x + (field_width * y)];
    int val;
    for(int w = -1; w <= 1; w++) {
        int width_to_test = x + w;
        for(int h = -1; h <= 1; h++) {
            int height_to_test = y + h;           
            if(is_part_of_field(width_to_test, height_to_test)) {
                val = width_to_test + (field_width * height_to_test);
                Vector2D adj(w, h);
                adj = (adj.get_ortho()).normalized();
                field[val] = adj * gravForce;
            }
        }
    }
}

auto Vector_Field::add_wall(int x, int y, int width, int height) -> void {
    for(int i = x; i < width; i++){
        for(int p = y; p < height; p++){
            field[i + (p * field_width)] = 0;
        }
    }
}


auto Vector_Field::get_force(int x, int y) -> Vector2D {
    if(is_part_of_field(x,y)) {
        return field[x + (field_width * y)];
    }
    return Vector2D(0,0);
}

auto Vector_Field::get_scale_factor() -> int {
    return field_scale_factor;        
}


auto Vector_Field::get_width() -> int {
    return field_width;
}


auto Vector_Field::get_height() -> int {
    return field_height;
}

auto Vector_Field::get_total_vectors() -> int {
    return field_height * field_width;
}


Vector_Field::~Vector_Field() {
    field_height = 0;
    field_width = 0;
}
