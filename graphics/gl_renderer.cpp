#include "./gl_renderer.h"

const char *vertex_shader_src =
    "uniform mat3 u_proj;"
    "attribute vec2 a_pos;"
    "void main() {"
    "   gl_Position = vec4(u_proj * vec3(a_pos, 1.0), 1.0);"
    "}";

const char *fragment_shader_src =
    "void main() {"
    "   gl_FragColor = vec4(1, 1, 1, 1);"
    "}";


auto GL_Renderer::compile_shader(int type, const char *src) -> GLuint {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::cerr << "Error compiling shader " << type << std::endl;
    }

    return shader;
}


auto GL_Renderer::link_program(GLuint v_shader, GLuint f_shader) -> GLuint {
    GLuint program = glCreateProgram();
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::cerr << "Error linking program" << std::endl;
    }
}


GL_Renderer::GL_Renderer(int vec_width, int vec_height)
    : vec_width(vec_width), vec_height(vec_height)
{
}


GL_Renderer::~GL_Renderer() {
}


auto GL_Renderer::setup() -> void {
    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glGenBuffers(1, &this->m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_buffer);

    long n_verticies = vec_width * vec_height * 4;
    m_vertex_data = std::unique_ptr<float[]>(new float[n_verticies * 2]);
    for (long i = 0; i < n_verticies; i++) {
        m_vertex_data[i * 2 + 0] = 0.0f;
        m_vertex_data[i * 2 + 1] = 0.0f;
    }

    float *v_data = m_vertex_data.get();
    glBufferData(GL_ARRAY_BUFFER, n_verticies * 2 * 4, v_data, GL_DYNAMIC_DRAW);

    long n_indicies = vec_width * vec_height * 6;
    std::unique_ptr<GLuint[]> index_data(new GLuint[n_indicies]);
    int j = 0;
    for (int i = 0; i < vec_width * vec_height; i++) {
        index_data[i * 6 + 0] = j + 0;
        index_data[i * 6 + 1] = j + 2;
        index_data[i * 6 + 2] = j + 1;
        index_data[i * 6 + 3] = j + 0;
        index_data[i * 6 + 4] = j + 3;
        index_data[i * 6 + 5] = j + 2;
    }

    glGenBuffers(1, &this->m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(m_index_buffer, n_indicies * 4, index_data.get(), GL_STATIC_DRAW);

    m_vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
    m_fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);
    m_program = link_program(m_vertex_shader, m_fragment_shader);

    m_proj_mat_loc = glGetUniformLocation(m_program, "u_proj");
    m_a_pos_loc = glGetAttribLocation(m_program, "a_pos");

    glClearColor(1, 0, 0, 1);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CW);
}


auto GL_Renderer::rebuffer_data() -> void {
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vec_width * vec_height * 2 * 4, m_vertex_data.get());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


auto GL_Renderer::render() -> void {
    glClear(GL_COLOR_BUFFER_BIT);
}