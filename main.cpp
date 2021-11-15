#include <iostream>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define PI 3.14159265359
#define DEGREE(radians) radians*0.0174533

#define PLAYER_COLOR 1, 1, 0
#define PLAYER_ROT_VEL 0.1
#define PLAYER_FOV 60

#define MAP_BORDER_COLOR 1, 1, 1
#define EMPTY_COLOR 0.3, 0.3, 0.3

#define RAY_COLOR_1 0,1,0
#define RAY_COLOR_2 1,0,0

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 512

float player_x, player_y, player_angle;
float player_dx, player_dy;

/*
float distance(float ax, float ay, float bx, float by, float angle)
{
    return cos(angle)*(bx-ax) - sin(angle)*(by-ay);
}
*/

float distance(float ax, float ay, float bx, float by, float angle)
{
    return sqrt(pow(ax-bx,2) + pow(ay-by,2));
}

void buttons(unsigned char key, int x, int y)
{
    if (key == 'w')
    {
        player_y += player_dy;
        player_x += player_dx;
    }

    if (key == 's')
    {
        player_y -= player_dy;
        player_x -= player_dx;
    }

    if (key == 'a')
    {
        player_angle -= PLAYER_ROT_VEL;

        if (player_angle < 0)
            player_angle += 2 * PI;

        player_dx = cos(player_angle) * 5;
        player_dy = sin(player_angle) * 5;
    }

    if (key == 'd')
    {
        player_angle += PLAYER_ROT_VEL;

        if (player_angle > 2 * PI)
            player_angle -= 2 * PI;

        player_dx = cos(player_angle) * 5;
        player_dy = sin(player_angle) * 5;
    }
    glutPostRedisplay();
}

float map_x = 8, map_y = 8, map_s = map_x * map_y;
int map[] = {
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    1,
    0,
    0,
    0,
    0,
    1,
    1,
    0,
    1,
    0,
    0,
    0,
    0,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    0,
    1,
    0,
    0,
    1,
    0,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
};

void draw2DMap()
{
    int x, y, x0, y0;
    for (y = 0; y < map_y; y++)
    {

        for (x = 0; x < map_x; x++)
        {

            if (map[y * (int)map_x + x] == 1)
            {
                glColor3f(MAP_BORDER_COLOR);
            }
            else
            {
                glColor3f(EMPTY_COLOR);
            }

            x0 = x * map_s;
            y0 = y * map_s;

            glBegin(GL_QUADS);

            glVertex2i(x0 + 1, y0 + 1);
            glVertex2i(x0 + 1, y0 + map_s - 1);
            glVertex2i(x0 + map_s - 1, y0 + map_s - 1);
            glVertex2i(x0 + map_s - 1, y0 + 1);

            glEnd();
        }
    }
}

void draw3DRays()
{
    int r, mp, mx, my;
    float ray_x, ray_y, ray_angle, xo, yo;

    ray_angle = player_angle - DEGREE((int)PLAYER_FOV/2);

    // keep ray_angle always beteen 0 and 2 pi
    if(ray_angle < 0) 
        ray_angle += 2*PI;
    if(ray_angle > 2*PI) 
        ray_angle -= 2*PI; 
    
    // TODO ADD MORE RAYS (it checks for each ray)
    for (r = 0; r < PLAYER_FOV; r++)
    {
        // Checking for horizontal ray collisions
        int dof = 0;

        float aTan = -1 / tan(ray_angle);
        
        float horizontalDistance = 1000000;
        float horizontal_x = player_x;
        float horizontal_y = player_y;

        // if the player is looking up
        if (ray_angle > PI)
        {
            ray_y = (((int)player_y >> 6) << 6) - 0.0001;
            ray_x = (player_y - ray_y) * aTan + player_x;

            yo = -64;
            xo = -yo * aTan;
        }

        if (ray_angle < PI)
        {
            ray_y = (((int)player_y >> 6) << 6) + 64;
            ray_x = (player_y - ray_y) * aTan + player_x;

            yo = 64;
            xo = -yo * aTan;
        }

        // if the player is looking staight left or right (never will cross an horizontal guide line)
        if (ray_angle == 0 || ray_angle == PI)
        {
            ray_x = player_x;
            ray_y = player_y;
            dof = 8;
        }

        while (dof < 8)
        {
            mx = (int)ray_x >> 6;
            my = (int)ray_y >> 6;
            mp = my * map_x + mx;

            // if the rays hits an wall
            if (mp > 0 && mp < map_x * map_y && map[mp] == 1)
            {
                horizontal_x = ray_x;
                horizontal_y = ray_y;

                horizontalDistance = distance(player_x, player_y, horizontal_x, horizontal_y, ray_angle);

                dof = 8;
            } else {
                ray_x += xo;
                ray_y += yo;
                dof += 1;
            }

        }
        
        // Renders an ray of horizonal scan, useless by now :)
        // because we will render this if the distance between the
        // player and the wall is minor than the ray of vertical scan
        /*
        glColor3f(RAY_COLOR_1);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(player_x, player_y);
        glVertex2i(ray_x, ray_y);
        glEnd();
        */

        // Checking for vertical ray collisions
        dof = 0;
        float nTan = -tan(ray_angle);
 
        float verticalDistance = 1000000;
        float vertical_x = player_x;
        float vertical_y = player_y;

        // if the player is looking left 
        if (ray_angle > PI/2 && ray_angle < 1.5*PI)
        {
            ray_x = (((int)player_x >> 6) << 6) - 0.0001;
            ray_y = (player_x - ray_x) * nTan + player_y;
            xo = -64;
            yo = -xo * nTan;
        }

        // if the player is looking right
        if (ray_angle < PI/2 || ray_angle > 1.5*PI)
        {
            ray_x = (((int)player_x >> 6) << 6) + 64;
            ray_y = (player_x - ray_x) * nTan + player_y;
  
            xo = 64;
            yo = -xo * nTan;
        }
  
        // if the player is looking staight left or right (never will cross an horizontal guide line)
        if (ray_angle == 0 || ray_angle == PI)
        {
            ray_x = player_x;
            ray_y = player_y;
            dof = 8;
        }

        while (dof < 8) 
        {
            mx = (int)ray_x >> 6;
            my = (int)ray_y >> 6;
            mp = my * map_x + mx;
            
            // if the rays hits an wall
            if (mp > 0 && mp < map_x * map_y && map[mp] == 1) 
            {

                vertical_x = ray_x;
                vertical_y = ray_y;

                verticalDistance = distance(player_x, player_y, vertical_x, vertical_y, ray_angle);

                dof = 8;
            } else {
                ray_x += xo;
                ray_y += yo;
                dof += 1;
            }
        }

        if(verticalDistance < horizontalDistance)
        {
            ray_x = vertical_x;
            ray_y = vertical_y;
        }
        if( verticalDistance > horizontalDistance )
        {
            ray_x = horizontal_x;
            ray_y = horizontal_y;
        }
        
        glColor3f(RAY_COLOR_2);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2i(player_x, player_y);
        glVertex2i(ray_x, ray_y);
        glEnd();

        ray_angle += DEGREE(1);
        if(ray_angle < 0)
            ray_angle += 2*PI;
        if(ray_angle > 2*PI)
            ray_angle -= 2*PI;

    } 

}

void drawPlayer()
{
    glColor3f(PLAYER_COLOR);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(player_x, player_y);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(player_x, player_y);
    glVertex2i(player_x + player_dx * 5, player_y + player_dy * 5);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw2DMap();
    drawPlayer();
    draw3DRays();
    glutSwapBuffers();
}

void init()
{
    glClearColor(0.3, 0.3, 0.3, 0.3);
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    player_x = 300;
    player_y = 300;

    player_angle = 0;
    player_dx = cos(player_angle) * 5;
    player_dy = sin(player_angle) * 5;
}

int main(int argc, char *argv[])
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
