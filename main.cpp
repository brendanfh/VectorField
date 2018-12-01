#include <iostream>

#include "graphics/glfw_window.h"
#include "physics/vector_field/vector_field.h"

auto main() -> int {


    Vector_Field test(10, 10, 10);

    test.add_gravity_well(0,0,10.0f);
    std::cout << "position (1,1): " << test.get_force(1,1).to_string() << std::endl;
    std::cout << "position (0,1): " << test.get_force(0,1).to_string() << std::endl;
    std::cout << "position (1,0): " << test.get_force(1,0).to_string() << std::endl;

    GLFW_Window window(800, 600);
    window.create();

    while (!window.should_close()) {
        window.poll_events();
    }

    return 0;
}