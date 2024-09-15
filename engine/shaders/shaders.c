
#include "shaders.h"

#include <stdio.h>
#include <stdlib.h>

const GLchar *VertexShader =
    {
        "#version 400\n"

        "layout(location=0) in vec4 in_Position;\n"
        "layout(location=1) in vec4 in_Color;\n"
        "out vec4 ex_Color;\n"

        "void main(void)\n"
        "{\n"
        "  gl_Position = in_Position;\n"
        "  ex_Color = in_Color;\n"
        "}\n"};

const GLchar *FragmentShader =
    {
        "#version 400\n"

        "in vec4 ex_Color;\n"
        "out vec4 out_Color;\n"

        "void main(void)\n"
        "{\n"
        "  out_Color = ex_Color;\n"
        "}\n"};

Shader create_shader()
{
    GLenum ErrorCheckValue = glGetError();

    int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &VertexShader, NULL);
    glCompileShader(vertex_shader_id);

    int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &FragmentShader, NULL);
    glCompileShader(fragment_shader_id);

    int program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);
    glUseProgram(program_id);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
            stderr,
            "ERROR: Could not create the shaders: %s \n",
            gluErrorString(ErrorCheckValue));

        exit(EXIT_FAILURE);
    }

    Shader shader = {vertex_shader_id, fragment_shader_id, program_id};
    return shader;
}

void destroy_shader(Shader shader)
{
    GLenum ErrorCheckValue = glGetError();

    glUseProgram(0);

    glDetachShader(shader.program_id, shader.vertex_shader_id);
    glDetachShader(shader.program_id, shader.fragment_shader_id);

    glDeleteShader(shader.fragment_shader_id);
    glDeleteShader(shader.vertex_shader_id);

    glDeleteProgram(shader.program_id);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
            stderr,
            "ERROR: Could not destroy the shaders: %s \n",
            gluErrorString(ErrorCheckValue));

        exit(EXIT_FAILURE);
    }
}