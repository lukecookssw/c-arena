
#include "vbo.h"

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

VBO create_vbo(GLfloat *vertices, size_t vertex_count, GLfloat *colours, size_t colour_count)
{
    GLenum ErrorCheckValue = glGetError();
    GLuint vao_id, vbo_id, colour_buffer_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id); // create N buffers on the GPU and store pointers to them in the array

    // bind the buffer to the context and "activate" the current buffer object as the target
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    // copy the data from the CPU to the GPU
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // DESCRIBE the data you just sent (e.g. tell the GPU how to
    // interpret the data in the buffer)
    // See: https://openglbook.com/chapter-2-vertices-and-shapes.html
    glVertexAttribPointer(
        0,        // USER-MAINTAINED index of the vertex attribute that is to be modified. Match with layout(location = 0) in the vertex shader
        4,        // the "size" which is the number of components per attribute (1, 2, 3, 4). These are x,y,z,w for the vertices
        GL_FLOAT, // the type of the data
        GL_FALSE, // Normalize? e.g. will covert to a float value between -1->1 or 0->1
        0,        // stride: the number of bytes between the start of the current vertex and the start of the next vertex
        0);       // offset: the number of bytes from the start of the current vertex to the start of the attribute

    // enable the vertex attribute array at the specified index
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &colour_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, colour_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, colour_count * sizeof(GLfloat), colours, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
            stderr,
            "ERROR: Could not create a VBO: %s \n",
            gluErrorString(ErrorCheckValue));

        exit(-1);
    }

    VBO vbo = {vao_id, vbo_id, colour_buffer_id};
    return vbo;
}

void destroy_vbo(VBO vbo)
{
    GLenum ErrorCheckValue = glGetError();

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo.colour_buffer_id);
    glDeleteBuffers(1, &vbo.vbo_id);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vbo.vao_id);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
            stderr,
            "ERROR: Could not destroy the VBO: %s \n",
            gluErrorString(ErrorCheckValue));

        exit(-1);
    }
}