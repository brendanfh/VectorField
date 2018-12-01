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
    //pthread_barrier_init(&update_barrier, NULL, NUM_THREADS + 1);
    //pthread_t id[NUM_THREADS];
    //int thread_id_parameter[NUM_THREADS];

    /*
    for(int i = 0; i < NUM_THREADS; i++){
        thread_id_parameter[i] = i;
        pthread_create(&id[i], NULL, threading, &thread_id_parameter[i]);
    }
    */

    Body* bodies = new Body[10000];
    for (int i = 0; i < 10000; i++) {
        bodies[i].set_pos(((float)(rand() % 1000)), ((float)(rand() % 1000)));
    }

    field = new Vector_Field(10,100,100);

    GLFW_Window window(800, 600);
    window.create();

    GL_Renderer gl_renderer(100, 100);
    gl_renderer.setup();

    int t = 0;
    while (!window.should_close()) {
        t += 1;
        field->add_gravity_well(30, 30, 1.0f);
        //field->add_gravity_well(35, 30, 1.0f);
        //field->add_gravity_well(60, 40, 2.0f);
        //field->add_gravity_well(20, 90, 2.0f);
        //field->add_gravity_well(99, 99, 2.5f);

        field->add_gravity_well(50, 50, 1.f);
        //field->add_curl(t, 55, (float)sin(t / 5.0f) * 3);
        field->add_curl(50, 60, -.5f);

        field->add_explosion((int)(sin(t / 120.f) * 40) + 50, 90, 4.f);
        field->add_explosion(-(int)(sin(t / 120.f) * 40) + 50, 10, 4.f);
        field->add_explosion(90, (int)(sin(t / 120.f) * 40) + 50, 4.f);
        field->add_explosion(10, -(int)(sin(t / 120.f) * 40) + 50, 4.f);

        field->step();

        for (int i = 0; i < 10000; i++) {
            Vector2D pos = bodies[i].get_force_position(10);
            bodies[i].apply_force(field->get_force((int)pos.x, (int)pos.y));
            bodies[i].update(0.06);

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