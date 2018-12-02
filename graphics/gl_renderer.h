#ifndef _GL_RENDERER_
#define _GL_RENDERER_

#ifdef APPLE
#include <OpenGL/gl3.h>
#else
#include <GLES3/gl3.h>
#endif

#include <iostream>
#include <memory>
#include "../utils/transform2D.h"
#include "../physics/vector_field/vector_field.h"

class GL_Renderer {
    private:
        float* m_vertex_data;
        float* m_particle_data;

        GLuint m_vertex_buffer, m_index_buffer;
        GLuint m_particle_buffer;

        GLuint m_vertex_shader, m_fragment_shader;

        GLuint m_program;

        GLint m_proj_mat_loc, m_a_pos_loc, m_u_col_loc;

        int vec_width;
        int vec_height;

        auto compile_shader(int type, const char *src) -> GLuint;
        auto link_program(GLuint v_shader, GLuint f_shader) -> GLuint;

    public:
        GL_Renderer(int vec_width, int vec_height);
        ~GL_Renderer();

        auto setup() -> void;
        auto rebuffer_data() -> void;

        auto update_field(Vector_Field& vf) -> void;
        auto update_particle(int index, float x, float y) -> void;

        auto render(Transform2D& transform) -> void;
};

#endif
