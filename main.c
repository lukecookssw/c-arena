
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
    GLfloat vertices[] = {
        //  x,     y,    z,    w
        -0.8f, -0.8f, 0.0f, 1.0f,
        0.0f, 0.8f, 0.0f, 1.0f,
        0.8f, -0.8f, 0.0f, 1.0f};

    GLfloat colours[] = {
        // r,    g,    b,    a
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f};
    
    // Calculate the number of vertices and colors
    size_t vertex_count = sizeof(vertices) / sizeof(vertices[0]);
    size_t colour_count = sizeof(colours) / sizeof(colours[0]);

    VBO vbo = create_vbo(vertices, vertex_count, colours, colour_count);
    VaoId           = vbo.vao_id;
    VboId           = vbo.vbo_id;
    ColorBufferId   = vbo.colour_buffer_id;
}

void DestroyVBO(void)
{
    VBO vbo = {VaoId, VboId, ColorBufferId};
    destroy_vbo(vbo);
}

void CreateShaders(void)
{
    Shader shader       = create_shader();
    VertexShaderId      = shader.vertex_shader_id;
    FragmentShaderId    = shader.fragment_shader_id;
    ProgramId           = shader.program_id;
}

void DestroyShaders(void)
{
    Shader shader = {VertexShaderId, FragmentShaderId, ProgramId};
    destroy_shader(shader);
}