#include <iostream>

#include <pthread.h>
#include <unistd.h>
#include <cmath>

#include "graphics/gl_renderer.h"
#include "graphics/glfw_window.h"
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

    pthread_barrier_init(&update_barrier, NULL, NUM_THREADS + 1);
    pthread_t id[NUM_THREADS];
    int thread_id_parameter[NUM_THREADS];

    /*
    for(int i = 0; i < NUM_THREADS; i++){
        thread_id_parameter[i] = i;
        pthread_create(&id[i], NULL, threading, &thread_id_parameter[i]);
    }
    */

    field = new Vector_Field(10,100,100);

//    field->add_gravity_well(0,0,10.0f);
    std::cout << "position (1,1): " << field->get_force(1,1).to_string() << std::endl;
    std::cout << "position (0,1): " << field->get_force(0,1).to_string() << std::endl;
    std::cout << "position (1,0): " << field->get_force(1,0).to_string() << std::endl;

//    field->step();
    std::cout << "position (2,2): " << field->get_force(2,2).to_string() << std::endl;
    

    GLFW_Window window(800, 600);
    window.create();

    GL_Renderer gl_renderer(100, 100);
    gl_renderer.setup();

    int t = 0;
    int n = 60;

    while (!window.should_close()) {
        n--;
        if (n == 0) {
            t += 1;
            n = 50;
        }
        field->add_gravity_well(30, 30, 1.0f);
        field->add_gravity_well(35, 30, 1.0f);
        field->add_gravity_well(60, 40, 2.0f);
        field->add_gravity_well(20, 90, 2.0f);
        field->add_gravity_well(99, 99, 2.5f);

        field->add_curl(50, 50, 5.f);
        field->add_curl(t, 55, (float)sin(t / 5.0f) * 3);
        field->add_curl(50, 60, -3.f);

        field->step();

        gl_renderer.update_field(*field);
        gl_renderer.rebuffer_data();
        gl_renderer.render();
        window.poll_events();

        usleep(1000);
    }

    return 0;
}