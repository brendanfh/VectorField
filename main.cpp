#include <iostream>

#include <pthread.h>
#include <unistd.h>
#include <cmath>

#include "graphics/gl_renderer.h"
#include "graphics/glfw_window.h"
#include "utils/transform2D.h"
#include "physics/body/body.h"
#include "physics/vector_field/vector_field.h"

#define NUM_THREADS 7

#define FIELD_WIDTH 800
#define FIELD_HEIGHT 800
#define FIELD_SCALE_FACTOR 10
#define BODY_COUNT 20000

pthread_barrier_t force_barr;
pthread_barrier_t swap_barr;
pthread_barrier_t update_barr;

class Main {
	private:
		GLFW_Window window;
		Transform2D trans;

	public:
		Vector_Field* field;
		Body* bodies;
		GL_Renderer gl_renderer;

		Main();
		~Main();

		auto update(int t) -> void;
		auto draw() -> void;

		auto get_window() -> GLFW_Window&;
};

Main::Main()
	: field(new Vector_Field(FIELD_SCALE_FACTOR, FIELD_WIDTH, FIELD_HEIGHT))
	, bodies(new Body[BODY_COUNT])
	, window(1500, 1000)
	, gl_renderer(FIELD_WIDTH, FIELD_HEIGHT)
{
    for (int i = 0; i < BODY_COUNT; i++) {
        bodies[i].set_pos(((float)(rand() % (FIELD_WIDTH * FIELD_SCALE_FACTOR))), ((float)(rand() % (FIELD_HEIGHT * FIELD_SCALE_FACTOR))));
    }

	window.create();
	gl_renderer.setup();

	trans.scale(2.0f / (FIELD_WIDTH * FIELD_SCALE_FACTOR), -2.0 / (FIELD_HEIGHT * FIELD_SCALE_FACTOR));
	trans.translate(-1, 1);
}

Main::~Main() {
	delete field;
	delete[] bodies;
}

auto Main::get_window() -> GLFW_Window& {
	return window;
}

auto Main::update(int t) -> void {

	//field->step();

	//for (int i = 0; i < BODY_COUNT; i++) {
	//	Vector2D pos = bodies[i].get_force_position(10);
	//	bodies[i].apply_force(field->get_force((int)pos.x, (int)pos.y));
	//	bodies[i].update(.8);

	//	Vector2D new_pos = bodies[i].get_position();
	//	gl_renderer.update_particle(i, new_pos.x, new_pos.y);
	//}

	if (window.is_key_down(GLFW_KEY_LEFT)) {
		trans.translate(0.004f, 0.f);
	}

	if (window.is_key_down(GLFW_KEY_RIGHT)) {
		trans.translate(-0.004f, 0.f);
	}

	if (window.is_key_down(GLFW_KEY_UP)) {
		trans.translate(0.f, -0.004f);
	}

	if (window.is_key_down(GLFW_KEY_DOWN)) {
		trans.translate(0.f, 0.004f);
	}

	if (window.is_key_down(GLFW_KEY_Q)) {
		trans.scale(1.01f, 1.01f);
	}

	if (window.is_key_down(GLFW_KEY_A)) {
		trans.scale(.99f, .99f);
	}
}

auto Main::draw() -> void {
	gl_renderer.update_field(*field);
	gl_renderer.rebuffer_data();
	gl_renderer.render(trans);
	window.poll_events();
}

Main* mainSim;
void* step_threading(void* id) {
	long threadID = (long)id;
	std::cout << threadID << std::endl;
	int totalVectors = mainSim->field->get_total_vectors();
	while(1) {
		pthread_barrier_wait(&force_barr);
		for(long current_pos = threadID; current_pos < totalVectors; current_pos += NUM_THREADS) {
			mainSim->field->step(current_pos);
		}
		
		pthread_barrier_wait(&swap_barr);		
		if(threadID == 0) {
			mainSim->field->swap_fields();
		}
		pthread_barrier_wait(&swap_barr);		
		for (int i = threadID; i < BODY_COUNT; i+=NUM_THREADS) {
			Vector2D pos = mainSim->bodies[i].get_force_position(FIELD_SCALE_FACTOR);
			mainSim->bodies[i].apply_force(mainSim->field->get_force((int)pos.x, (int)pos.y));
			mainSim->bodies[i].update(.8);

			Vector2D new_pos = mainSim->bodies[i].get_position();
			mainSim->gl_renderer.update_particle(i, new_pos.x, new_pos.y);
		}
		pthread_barrier_wait(&update_barr);
	}	
}

auto main() -> int {
    srand(time(NULL));
	
	pthread_barrier_init(&force_barr, NULL, NUM_THREADS + 1);
	pthread_barrier_init(&swap_barr, NULL, NUM_THREADS);
	pthread_barrier_init(&update_barr, NULL, NUM_THREADS + 1);

	pthread_t threadIDs[NUM_THREADS];

	mainSim = new Main();

	for (long i = 0; i < NUM_THREADS; i++) {
		pthread_create(&threadIDs[i], NULL, step_threading, (void*) i);
	}

    int t = 0;
    while (!mainSim->get_window().should_close()) {
        t += 1;
		mainSim->field->add_curl(50, 100, 3.f);
		mainSim->field->add_inward_curl(150, 100, 3.f);
		mainSim->field->add_inward_curl((int)(sin(t / 360.f) * 80) + 100, (int)(cos(t / 720.f) * 40.f) + 68, 20.f);
		mainSim->field->add_gravity_well(20, 90, 2.0f);
		if (t % 4000 == 10)
			mainSim->field->add_explosion(rand() % 150 + 25, rand() % 150 + 25, 15.f);
		mainSim->field->add_curl(50, 100, 3.f);
		mainSim->field->add_inward_curl(150, 100, 3.f);
		mainSim->field->add_inward_curl((int)(sin(t / 360.f) * 80) + 100, (int)(cos(t / 720.f) * 40.f) + 68, 20.f);

		mainSim->field->add_gravity_well(500, 500, 60.f);

		mainSim->field->add_explosion(100, 180, 3.f);

		pthread_barrier_wait(&force_barr);
		pthread_barrier_wait(&update_barr);
		mainSim->update(t);
		mainSim->draw();

        usleep(1000);
    }

	delete mainSim;

    return 0;
}
