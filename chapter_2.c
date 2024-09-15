
#include "engine/game/game.h"

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

// Function prototypes
void Initialize(int, char *[]);
void InitWindow(int, char *[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void timer_function(int);
void idle_function(void);

void Cleanup(void);
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


void InitWindow(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutInitWindowSize(CurrentWidth, CurrentHeight);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

    if (WindowHandle < 1)
    {
        fprintf(
            stderr,
            "ERROR: Could not create a new rendering window.\n");
        exit(EXIT_FAILURE);
    }

    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);
    glutIdleFunc(idle_function);
    glutTimerFunc(0, timer_function, 0);
    glutCloseFunc(Cleanup);
}

void Cleanup(void)
{
    DestroyShaders();
    DestroyVBO();
}

void ResizeFunction(int Width, int Height)
{
    CurrentWidth = Width;
    CurrentHeight = Height;
    glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void RenderFunction(void)
{
    ++FrameCount;
    glBeginQuery(GL_TIME_ELAPSED, query);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glEndQuery(GL_TIME_ELAPSED);

    glutSwapBuffers();
    glutPostRedisplay();
}

void idle_function(void)
{
    glutPostRedisplay();
}

void print_ms_per_frame(char *str)
{
    // Get query result
    GLuint64 timeElapsed;
    glGetQueryObjectui64v(query, GL_QUERY_RESULT, &timeElapsed);

    // Convert nanoseconds to milliseconds
    double millisecondsPerFrame = timeElapsed / 1000000.0;

    // Milliseconds per frame
    sprintf(
        str,
        "%s: %.2f Milliseconds Per Frame @ %d x %d",
        WINDOW_TITLE_PREFIX,
        millisecondsPerFrame,
        CurrentWidth,
        CurrentHeight);
}

void print_fps(char *str)
{
    sprintf(
        str,
        "%s: %d Frames Per Second @ %d x %d",
        WINDOW_TITLE_PREFIX,
        FrameCount * 4,
        CurrentWidth,
        CurrentHeight);
}

void timer_function(int value)
{
    if (value != 0)
    {
        char *str = (char *)
            malloc(512 + strlen(WINDOW_TITLE_PREFIX));

        // print_ms_per_frame(str);
        print_fps(str);

        glutSetWindowTitle(str);
        free(str);
    }

    FrameCount = 0;
    glutTimerFunc(250, timer_function, 1);
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