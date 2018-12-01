#include "./gl_renderer.h"

const char *vertex_shader_src =
    "uniform mat3 u_proj;"
    "attribute vec2 a_pos;"
    "void main() {"
    "   gl_Position = vec4(u_proj * vec3(a_pos, 1.0), 1.0);"
    "}";

const char *fragment_shader_src =
    "uniform vec3 u_col;"
    "void main() {"
    "   gl_FragColor = vec4(u_col, 1);"
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

    return program;
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

    long n_verticies = vec_width * vec_height * 4;
    m_vertex_data = new float[n_verticies * 2];
    m_particle_data = new float[n_verticies * 2];
    for (long i = 0; i < n_verticies; i++) {
        m_vertex_data[i * 2 + 0] = 0.0f;
        m_vertex_data[i * 2 + 1] = 0.0f;
        m_particle_data[i * 2 + 0] = 0.0f;
        m_particle_data[i * 2 + 1] = 0.0f;
    }

    glGenBuffers(1, &this->m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, n_verticies * 2 * 4, m_vertex_data, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &this->m_particle_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_particle_buffer);
    glBufferData(GL_ARRAY_BUFFER, n_verticies * 2 * 4, m_particle_data, GL_DYNAMIC_DRAW);

    long n_indicies = vec_width * vec_height * 6;
    std::unique_ptr<GLuint[]> index_data(new GLuint[n_indicies]);
    int j = 0;
    for (int i = 0; i < vec_width * vec_height; i++) {
        index_data[i * 6 + 0] = j + 0;
        index_data[i * 6 + 1] = j + 1;
        index_data[i * 6 + 2] = j + 2;
        index_data[i * 6 + 3] = j + 0;
        index_data[i * 6 + 4] = j + 2;
        index_data[i * 6 + 5] = j + 3;
        j += 4;
    }

    glGenBuffers(1, &this->m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indicies * 4, index_data.get(), GL_STATIC_DRAW);

    m_vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
    m_fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);
    m_program = link_program(m_vertex_shader, m_fragment_shader);

    m_proj_mat_loc = glGetUniformLocation(m_program, "u_proj");
    m_u_col_loc = glGetUniformLocation(m_program, "u_col");
    m_a_pos_loc = glGetAttribLocation(m_program, "a_pos");

    glClearColor(0, 0, 0, 1);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}


auto GL_Renderer::update_field(Vector_Field& vf) -> void {
    Vector2D vec;
    int ind;
    float scale_factor = (float) vf.get_scale_factor();

    float px, py, vx, vy, tx, ty;

    for (int y = 0, ym = vf.get_height(); y < ym; y++) {
        for (int x = 0, xm = vf.get_width(); x < xm; x++) {
            ind = y * xm + x;
            vec = vf.get_force(x, y);
            
            px = x * scale_factor;
            py = y * scale_factor;

            vx = px + vec.x * scale_factor;
            vy = py + vec.y * scale_factor;

            tx = py - vy;
            ty = vx - px;

            tx *= 0.1;
            ty *= 0.1;

            m_vertex_data[ind * 8 + 0] = px - tx;
            m_vertex_data[ind * 8 + 1] = py - ty;
            m_vertex_data[ind * 8 + 2] = px + tx;
            m_vertex_data[ind * 8 + 3] = py + ty;
            m_vertex_data[ind * 8 + 4] = vx + tx;
            m_vertex_data[ind * 8 + 5] = vy + ty;
            m_vertex_data[ind * 8 + 6] = vx - tx;
            m_vertex_data[ind * 8 + 7] = vy - ty;
        }
    }
}


auto GL_Renderer::update_particle(int index, float x, float y) -> void {
    int size = 3;
    m_particle_data[index * 8 + 0] = x + 0;
    m_particle_data[index * 8 + 1] = y + 0;
    m_particle_data[index * 8 + 2] = x + size;
    m_particle_data[index * 8 + 3] = y + 0;
    m_particle_data[index * 8 + 4] = x + size;
    m_particle_data[index * 8 + 5] = y + size;
    m_particle_data[index * 8 + 6] = x + 0;
    m_particle_data[index * 8 + 7] = y + size;
}


auto GL_Renderer::rebuffer_data() -> void {
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vec_width * vec_height * 4 * 2 * 4, m_vertex_data);
    glBindBuffer(GL_ARRAY_BUFFER, m_particle_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vec_width * vec_height * 4 * 2 * 4, m_particle_data);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


auto GL_Renderer::render() -> void {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    const float mat[9] = {
        2.f / 1000.f, 0.f, 0.f,
        0.f, -2.f / 1000.f, 0.f,
        -1.f, 1.f, 1.f
    };

    glUniformMatrix3fv(m_proj_mat_loc, 1, GL_FALSE, (const GLfloat*) mat);

    glUseProgram(m_program);
    glEnableVertexAttribArray(m_a_pos_loc);
    
    glUniform3f(m_u_col_loc, 1, 1, 1);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glVertexAttribPointer(m_a_pos_loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*) 0);

    glDrawElements(GL_TRIANGLES, 6 * vec_width * vec_height, GL_UNSIGNED_INT, (void*) 0);

    glUniform3f(m_u_col_loc, 1, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_particle_buffer);
    glVertexAttribPointer(m_a_pos_loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*) 0);
    glDrawElements(GL_TRIANGLES, 6 * vec_width * vec_height, GL_UNSIGNED_INT, (void*) 0);

    glDisableVertexAttribArray(m_a_pos_loc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}