#ifndef _GL_RENDERER_
#define _GL_RENDERER_

#include <GLES3/gl3.h>
#include <iostream>
#include <memory>

class GL_Renderer {
    private:
        std::unique_ptr<float[]> m_vertex_data;

        GLuint m_vertex_buffer, m_index_buffer;
        GLuint m_vertex_shader, m_fragment_shader;

        GLuint m_program;

        GLint m_proj_mat_loc, m_a_pos_loc, m_a_col_loc;

        int vec_width;
        int vec_height;

        auto compile_shader(int type, const char *src) -> GLuint;
        auto link_program(GLuint v_shader, GLuint f_shader) -> GLuint;

    public:
        GL_Renderer(int vec_width, int vec_height);
        ~GL_Renderer();

        auto setup() -> void;
        auto rebuffer_data() -> void;

        auto render() -> void;
};

#endif