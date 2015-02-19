#include "gl_objects.h"
#include <GL/gl.h>

void DrawFrame(float origin_x, float origin_y, float origin_z, float frame_length)
{
    glBegin(GL_LINES);

    glColor3f(1,0,0);
    glVertex3f(origin_x, origin_y, origin_z);
    glVertex3f(origin_x+frame_length, origin_y, origin_z);

    glColor3f(0,0,1);
    glVertex3f(origin_x, origin_y, origin_z);
    glVertex3f(origin_x, origin_y+frame_length, origin_z);

    glColor3f(0,1,0);
    glVertex3f(origin_x, origin_y, origin_z);
    glVertex3f(origin_x, origin_y, origin_z+frame_length);

    glEnd();
}

void DrawCube(float origin_x, float origin_y, float origin_z, float
half_side)
{
    glBegin(GL_QUADS);
        // front
        glColor3f(0, 1, 0);
        glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z+half_side);
        glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z+half_side);
        glVertex3f(origin_x-half_side, origin_y+half_side,
origin_z+half_side);
        glVertex3f(origin_x-half_side, origin_y-half_side,
origin_z+half_side);
        // back
        glColor3f(0, 0.5, 0);
        glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z-half_side);
        glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z-half_side);
        glVertex3f(origin_x-half_side, origin_y-half_side,
origin_z-half_side);
        glVertex3f(origin_x-half_side, origin_y+half_side,
origin_z-half_side);
        // left
        glColor3f(0.5, 0, 0);
        glVertex3f(origin_x-half_side, origin_y-half_side,
origin_z-half_side);
        glVertex3f(origin_x-half_side, origin_y+half_side,
origin_z-half_side);
        glVertex3f(origin_x-half_side, origin_y+half_side,
origin_z+half_side);
        glVertex3f(origin_x-half_side, origin_y-half_side,
origin_z+half_side);
        // right
        glColor3f(1, 0, 0);
        glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z-half_side);
        glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z+half_side);
        glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z+half_side);
        glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z-half_side);
        // top
        glColor3f(0, 0, 1);
        glVertex3f(origin_x-half_side, origin_y+half_side,
origin_z+half_side);
        glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z+half_side);
        glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z-half_side);
        glVertex3f(origin_x-half_side, origin_y+half_side,
origin_z-half_side);
        // bottom
        glColor3f(0, 0, 0.5);
        glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z+half_side);
        glVertex3f(origin_x-half_side, origin_y-half_side,
origin_z+half_side);
        glVertex3f(origin_x-half_side, origin_y-half_side,
origin_z-half_side);
        glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z-half_side);
    glEnd();
}
