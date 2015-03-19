#ifndef AVATAR_H
#define AVATAR_H

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <events.h>
#include "sensor.h"

class CAvatar: public CEvent
{
private:
    // indique si l'avatar est en train de s'exécuter
    bool should_be_running;

    // indique si l'avatar doit être (re)modélisé
    bool needs_rendering;

    // dimensions et titre de la fenêtre
    int window_width;
    int window_height;
    const char* window_title;

    // la surface framebuffer correspondant à la fenêtre
    SDL_Surface* sdl_pimage;

    // l'ID de la texture
    GLuint texture;

    // coordonnées de l'origine
    double world_origin_x;
    double world_origin_y;
    double world_origin_z;
    double world_rx;
    double world_ry;

    // coordonnées de la scène
    double camera_tx;
    double camera_ty;
    double camera_min_tz;
    double camera_tz;
    double camera_aspect_ratio;
    double camera_min_z;
    double camera_max_z;
    double camera_fovy;

public:
    CAvatar();
    ~CAvatar();

    int OnExecute();
    bool OnInit();
    void OnCleanup();
    void OnLoop();
    void OnRender();

    void OnEvent(SDL_Event*);
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnResize(int w, int h);
    void OnExpose();
    void OnExit();
    void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
    void OnMouseWheel(bool, bool);

    void InitSceneConstants();
    void InitProjectionMatrix();

    void translate(double, double);
    void rotate(int, int);
    void zoom(double);
};

#endif // AVATAR_H
