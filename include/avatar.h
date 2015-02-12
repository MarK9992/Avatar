#ifndef AVATAR_H
#define AVATAR_H

#include <SDL/SDL.h>

class CAvatar
{
private:
    // indique si l'avatar est en train de s'exécuter
    bool should_be_running = false;

    // dimensions et titre de la fenêtre
    int window_width = 640;
    int window_height = 480;
    const char* window_title = "My Avatar";

    // la surface framebuffer correspondant à la fenêtre
    SDL_Surface* sdl_pimage = NULL;
public:
    CAvatar();
    ~CAvatar();
    int OnExecute();

    bool OnInit();

    void OnCleanup();
    void OnLoop();
    void OnRender();

    void OnEvent(SDL_Event*);
};

#endif // AVATAR_H
