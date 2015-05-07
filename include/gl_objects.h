#ifndef GL_OBJECTS_H
#define GL_OBJECTS_H

#include <GL/gl.h>

GLuint Load2DTexture(GLsizei width, GLsizei height, int BytesPerPixel, const GLvoid * data);
void DrawFrame(float origin_x, float origin_y, float origin_z, float frame_length);
void DrawCube(float origin_x, float origin_y, float origin_z, float half_side);
void DrawCubeWithTexture(float origin_x, float origin_y, float origin_z, float half_side, GLuint texture);
void FillWindowWithTexture(GLuint texture_ID);

#endif // GL_OBJECTS_H
