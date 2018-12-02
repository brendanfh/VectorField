#include <iostream>

#include <pthread.h>
#include <unistd.h>
#include <cmath>

#include "graphics/gl_renderer.h"
#include "graphics/glfw_window.h"
#include "physics/body/body.h"
#include "physics/vector_field/vector_field.h"

#define NUM_THREADS 4

pthread_barrier_t update_barrier;

Vector_Field* field;
void* threading(void* test){
    long threadID = long(test);
    int totalVectors = field->get_total_vectors();
    while(1)
    {
        for(int i = threadID; i < totalVectors; i += NUM_THREADS)
        {
            field->step(i);
        }

        pthread_barrier_wait(&update_barrier);
    }
}

auto main() -> int {
    srand(time(NULL));

    Body* bodies = new Body[10000];
    for (int i = 0; i < 10000; i++) {
        bodies[i].set_pos(((float)(rand() % 2000)), ((float)(rand() % 2000)));
    }

    field = new Vector_Field(10,200,200);

    GLFW_Window window(1500, 1000);
    window.create();

    GL_Renderer gl_renderer(200, 200);
    gl_renderer.setup();

    int t = 0;
    while (!window.should_close()) {
        t += 1;
        //field->add_gravity_well(30, 30, 1.0f);
        //field->add_gravity_well(35, 30, 1.0f);
        //field->add_gravity_well(60, 40, 2.0f);
        //field->add_gravity_well(99, 99, 2.5f);
        //field->add_wall(100, 40, 20, 20);

        //field->add_gravity_well(50, 50, 1.f);
        ////field->add_curl(t, 55, (float)sin(t / 5.0f) * 3);
        //field->add_curl(50, 60, -.5f);

        //field->add_explosion((int)(sin(t / 120.f) * 40) + 50, 90, 4.f);
        //field->add_explosion(-(int)(sin(t / 120.f) * 40) + 50, 10, 4.f);
        //field->add_explosion(90, -(int)(sin(t / 120.f) * 40) + 50, 4.f);
        //field->add_explosion(10, (int)(sin(t / 120.f) * 40) + 50, 4.f);
        //field->add_gravity_well(50, 90, 3.f);

        //field->add_curl(100, 100, 1.f);

        //field->add_gravity_well(100, 100, 5.0f);

        //field->add_explosion(100, 100, 9001.f);

        field->add_curl(50, 100, 3.f);
        field->add_inward_curl(150, 100, 3.f);
        field->add_inward_curl((int)(sin(t / 360.f) * 80) + 100, (int)(cos(t / 720.f) * 40.f) + 68, 20.f);
        field->add_gravity_well(20, 90, 2.0f);
        if (t % 1000 == 10)
            field->add_explosion(rand() % 150 + 25, rand() % 150 + 25, 15.f);
        field->add_curl(50, 100, 3.f);
        field->add_inward_curl(150, 100, 3.f);
        field->add_inward_curl((int)(sin(t / 360.f) * 80) + 100, (int)(cos(t / 720.f) * 40.f) + 68, 20.f);

        field->step();

        for (int i = 0; i < 10000; i++) {
            Vector2D pos = bodies[i].get_force_position(10);
            bodies[i].apply_force(field->get_force((int)pos.x, (int)pos.y));
            bodies[i].update(0.2);

            Vector2D new_pos = bodies[i].get_position();
            gl_renderer.update_particle(i, new_pos.x, new_pos.y);
        }

        gl_renderer.update_field(*field);
        gl_renderer.rebuffer_data();
        gl_renderer.render();
        window.poll_events();

        usleep(1000);
    }

    return 0;
}