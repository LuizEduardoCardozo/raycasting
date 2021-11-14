#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(640, 560);
    glutCreateWindow("olá, mundo!");
    glutDisplayFunc(display);
    gluOrtho2D(0, 640, 0, 560);

    glutMainLoop();
    return 0;
}


