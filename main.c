
#include "engine/game/game.h"
#include "engine/vbo/vbo.h"
#include "engine/shaders/shaders.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

GLuint VertexShaderId,
    FragmentShaderId,
    ProgramId,
    VaoId,
    VboId,
    ColorBufferId;

// Function prototypes
void CreateVBO(void);
void DestroyVBO(void);
void CreateShaders(void);
void DestroyShaders(void);

int main(int argc, char *argv[])
{
    game_start(argc, argv);
    CreateShaders();
    CreateVBO();
    glutMainLoop();

    game_end();
    return 0;
}


// Vertex Buffer Object
void CreateVBO(void)
{
    GLfloat Vertices[] = {
        //  x,     y,    z,    w
        -0.8f, -0.8f, 0.0f, 1.0f,
        0.0f, 0.8f, 0.0f, 1.0f,
        0.8f, -0.8f, 0.0f, 1.0f};

    GLfloat Colors[] = {
        // r,    g,    b,    a
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f};

    GLenum ErrorCheckValue = glGetError();

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    // Note: VboId is declared as a single UInt32 here, but it can/should be an array of UInt32s
    // so that multiple VBOs can be created and managed (and pointers to each of them would live in the array)
    // The demo uses a single GLuint because it only creates one VBO.
    glGenBuffers(1, &VboId); // create N buffers on the GPU and store pointers to them in the array

    // bind the buffer to the context and "activate" the current buffer object as the target
    glBindBuffer(GL_ARRAY_BUFFER, VboId);

    // copy the data from the CPU to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

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

    glGenBuffers(1, &ColorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
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
}

void DestroyVBO(void)
{
    GLenum ErrorCheckValue = glGetError();

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ColorBufferId);
    glDeleteBuffers(1, &VboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);

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

void CreateShaders(void)
{
    GLenum ErrorCheckValue = glGetError();

    VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
    glCompileShader(VertexShaderId);

    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
    glCompileShader(FragmentShaderId);

    ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);
    glLinkProgram(ProgramId);
    glUseProgram(ProgramId);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
            stderr,
            "ERROR: Could not create the shaders: %s \n",
            gluErrorString(ErrorCheckValue));

        exit(-1);
    }
}

void DestroyShaders(void)
{
    GLenum ErrorCheckValue = glGetError();

    glUseProgram(0);

    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);

    glDeleteShader(FragmentShaderId);
    glDeleteShader(VertexShaderId);

    glDeleteProgram(ProgramId);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
            stderr,
            "ERROR: Could not destroy the shaders: %s \n",
            gluErrorString(ErrorCheckValue));

        exit(-1);
    }
}