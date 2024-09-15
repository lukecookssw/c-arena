#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

typedef struct
{
    GLuint vao_id;
    GLuint vbo_id;
    GLuint colour_buffer_id;
} VBO;

VBO create_vbo(GLfloat *vertices, size_t vertex_count, GLfloat *colours, size_t colour_count);
void destroy_vbo(VBO vbo);

#endif // VBO_H