
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define WINDOW_TITLE_PREFIX "People Watcher"

int CurrentWidth = 800,
    CurrentHeight = 600,
    WindowHandle = 0;

unsigned FrameCount = 0;

GLuint query;
GLint available       = 0;
GLuint64 elapsed_time = 0;

// Function prototypes
void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void timer_function(int);
void idle_function(void);

int main(int argc, char* argv[])
{
    Initialize(argc, argv);

    glutMainLoop();

    return 0;
}

void Initialize(int argc, char* argv[])
{
    GLenum GlewInitResult;

    InitWindow(argc, argv);

    GlewInitResult = glewInit();

    if (GlewInitResult != GLEW_OK)
    {
        fprintf(
            stderr,
            "ERROR: %s\n",
            glewGetErrorString(GlewInitResult)
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

void InitWindow(int argc, char* argv[])
{
  glutInit(&argc, argv);
  
  glutInitContextVersion(4, 0);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutSetOption(
    GLUT_ACTION_ON_WINDOW_CLOSE,
    GLUT_ACTION_GLUTMAINLOOP_RETURNS
  );
  
  glutInitWindowSize(CurrentWidth, CurrentHeight);

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

  if(WindowHandle < 1) {
    fprintf(
      stderr,
      "ERROR: Could not create a new rendering window.\n"
    );
    exit(EXIT_FAILURE);
  }

  glutReshapeFunc(ResizeFunction);
  glutDisplayFunc(RenderFunction);
  glutIdleFunc(idle_function);
  glutTimerFunc(0, timer_function, 0);
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

  glEndQuery(GL_TIME_ELAPSED);

  glutSwapBuffers();
  glutPostRedisplay();
}

void idle_function(void)
{
  glutPostRedisplay();
}

void print_ms_per_frame(char* str)
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
      CurrentHeight
    );
}

void print_fps(char* str)
{
    sprintf(
      str,
      "%s: %d Frames Per Second @ %d x %d",
      WINDOW_TITLE_PREFIX,
      FrameCount * 4,
      CurrentWidth,
      CurrentHeight
    );
}

void timer_function(int value)
{
  if (value != 0) {
    char* str = (char*)
      malloc(512 + strlen(WINDOW_TITLE_PREFIX));

    //print_ms_per_frame(str);
    print_fps(str);

    glutSetWindowTitle(str);
    free(str);
  }
  
  FrameCount = 0;
  glutTimerFunc(250, timer_function, 1);
}