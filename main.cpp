#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define PLAYER_COLOR 1,1,0
#define MAP_BORDER_COLOR 1,1,1
#define EMPTY_COLOR 0.3, 0.3, 0.3 

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 512


float player_x, player_y;

void buttons(unsigned char key, int x, int y)
{
    if (key == 'w') player_y -= 5;
    if (key == 's') player_y += 5;
    if (key == 'a') player_x -= 5;
    if (key == 'd') player_x += 5;
    glutPostRedisplay();

}

float map_x = 8, map_y = 8, map_s = map_x * map_y;
int map[] = {
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,1,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

void draw2DMap() {
    int x,y,x0,y0;
    for(y=0; y<map_y; y++) {
    
        for(x=0; x<map_x; x++) {
            
            if(map[y*(int)map_x+x] == 1) {
                glColor3f(MAP_BORDER_COLOR); 
            } else {
                glColor3f(EMPTY_COLOR);
            }
            
            x0 = x*map_s;
            y0 = y*map_s;

            glBegin(GL_QUADS);

            glVertex2i(x0         + 1, y0         + 1);
            glVertex2i(x0         + 1, y0 + map_s - 1);
            glVertex2i(x0 + map_s - 1, y0 + map_s - 1);
            glVertex2i(x0 + map_s - 1, y0         + 1);
            
            glEnd();
            
        }

    }
}

void drawPlayer()
{
    glColor3f(PLAYER_COLOR);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(player_x, player_y);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw2DMap();
    drawPlayer();
    glutSwapBuffers();
}

void init()
{
    glClearColor(0.3, 0.3, 0.3, 0.3);
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    
    player_x = 300;
    player_y = 300;
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("olá, mundo!");
    glutDisplayFunc(display);
    glutKeyboardFunc(buttons);
    init();
    glutMainLoop();
    return 0;
}


