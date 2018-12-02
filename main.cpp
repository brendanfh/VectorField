#include <iostream>

#include <pthread.h>
#include <unistd.h>
#include <cmath>

#include "graphics/gl_renderer.h"
#include "graphics/glfw_window.h"
#include "utils/transform2D.h"
#include "physics/body/body.h"
#include "physics/vector_field/vector_field.h"

class Main {
	private:
		Vector_Field* field;
		Body* bodies;
		GLFW_Window window;
		GL_Renderer gl_renderer;
		Transform2D trans;

	public:
		Main();
		~Main();

		auto update(int t) -> void;
		auto draw() -> void;

		auto get_window() -> GLFW_Window&;
};

Main::Main()
	: field(new Vector_Field(10, 200, 200))
	, bodies(new Body[10000])
	, window(1500, 1000)
	, gl_renderer(200, 200)
{
    for (int i = 0; i < 10000; i++) {
        bodies[i].set_pos(((float)(rand() % 2000)), ((float)(rand() % 2000)));
    }

	window.create();
	gl_renderer.setup();

	trans.scale(2.0f / 2000.0f, -2.0 / 2000.0f);
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
	field->add_curl(50, 100, 3.f);
	field->add_inward_curl(150, 100, 3.f);
	field->add_inward_curl((int)(sin(t / 360.f) * 80) + 100, (int)(cos(t / 720.f) * 40.f) + 68, 20.f);
	field->add_gravity_well(20, 90, 2.0f);
	if (t % 4000 == 10)
		field->add_explosion(rand() % 150 + 25, rand() % 150 + 25, 15.f);
	field->add_curl(50, 100, 3.f);
	field->add_inward_curl(150, 100, 3.f);
	field->add_inward_curl((int)(sin(t / 360.f) * 80) + 100, (int)(cos(t / 720.f) * 40.f) + 68, 20.f);

	field->add_explosion(100, 180, 3.f);

	field->step();

	for (int i = 0; i < 10000; i++) {
		Vector2D pos = bodies[i].get_force_position(10);
		bodies[i].apply_force(field->get_force((int)pos.x, (int)pos.y));
		bodies[i].update(.8);

		Vector2D new_pos = bodies[i].get_position();
		gl_renderer.update_particle(i, new_pos.x, new_pos.y);
	}

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

auto main() -> int {
    srand(time(NULL));

	Main main;

    int t = 0;
    while (!main.get_window().should_close()) {
        t += 1;

		main.update(t);
		main.draw();


        usleep(1000);
    }

    return 0;
}
