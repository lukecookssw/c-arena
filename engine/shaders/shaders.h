
#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>

extern const GLchar *VertexShader;
extern const GLchar *FragmentShader;

typedef struct
{
    int vertex_shader_id;
    int fragment_shader_id;
    int program_id;
} Shader;

Shader create_shader();
void destroy_shader(Shader shader);

#endif // SHADERS_H