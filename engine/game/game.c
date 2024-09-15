
#include "game.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

GLuint query;
int CurrentWidth = 800,
    CurrentHeight = 600,
    WindowHandle = 0;

unsigned FrameCount = 0;

// prototypes
void glut_init(int argc, char *argv[]);
void create_window(char *title);
void resize_window(int width, int height);
void render_frame();
void idle_function();
void timer_function(int value);
void print_fps(char *str);
void cleanup();


void game_start(int argc, char *argv[])
{
    GLenum glew_init_result;
    glut_init(argc, argv);
    create_window("Hello world");
    glew_init_result = glewInit();
    if (glew_init_result != GLEW_OK)
    {
        fprintf(
            stderr,
            "ERROR: %s\n",
            glewGetErrorString(glew_init_result)
        );
        exit(EXIT_FAILURE);
    }
    
    fprintf(
        stdout,
        "INFO: OpenGL Version: %s\n",
        glGetString(GL_VERSION)
    );

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glGenQueries(1, &query);
}

void game_end()
{

}

void glut_init(int argc, char *argv[])
{
    // initialize the FreeGLUT library
    glutInit(&argc, argv);

    // set openGL version stuff
    glutInitContextVersion(4, 0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    // explicitly set some behaviour so that glut returns to
    // main when finished, rather than quitting the whole app
    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    // set some config
    glutInitWindowSize(CurrentWidth, CurrentHeight);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
}

void create_window(char *title)
{
    WindowHandle = glutCreateWindow(title);

    if (WindowHandle < 1)
    {
        fprintf(
            stderr,
            "ERROR: Could not create a new rendering window.\n");
        exit(EXIT_FAILURE);
    }

    // TODO: Enable fullscreen
    //glutFullScreen();

    // wire up callbacks
    glutReshapeFunc(resize_window);
    glutDisplayFunc(render_frame);
    glutIdleFunc(idle_function);
    glutTimerFunc(0, timer_function, 0);
    glutCloseFunc(cleanup);
}

void resize_window(int width, int height)
{
    CurrentWidth = width;
    CurrentHeight = height;
    glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void render_frame(void)
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

void timer_function(int value)
{
    if (value != 0)
    {
        char str[200];
        print_fps(str);
        glutSetWindowTitle(str);
    }

    FrameCount = 0;
    glutTimerFunc(1000, timer_function, 1);
}

void print_fps(char *str)
{
    sprintf(str, "C-ARENA: %d Frames Per Second @ %d x %d", FrameCount, CurrentWidth, CurrentHeight);
}

void cleanup(void)
{
    // TODO: cleanup
    // destroy_shaders();
    // destroy_vbos();
}