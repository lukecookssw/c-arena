#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

void create_vbo(GLfloat *vertices, size_t vertex_count, GLfloat *colours, size_t colour_count, GLuint *vao_id, GLuint *vbo_id, GLuint *colour_buffer_id);
void destroy_vbo(GLuint vao_id, GLuint vbo_id, GLuint colour_buffer_id);

#endif // VBO_H