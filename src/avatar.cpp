#include "avatar.h"
#include <iostream>

#define SDL_VIDEO_MODE_OPTIONS (SDL_RESIZABLE | SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER /*| SDL_OPENGL*/)
#define SDL_DEPTH 32

CAvatar::CAvatar() {}
CAvatar::~CAvatar() {}

int CAvatar::OnExecute()
{
    if(OnInit() == false) {
        return -1;
    }

    SDL_Event event;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    should_be_running = true;

    while(should_be_running) {
        while(SDL_PollEvent(&event)) {
            OnEvent(&event);
        }
        OnLoop();
        OnRender();
    }
    OnCleanup();

    return 0;
}

bool CAvatar::OnInit()
{
    char sdl_wdw_pos[] = "SDL_VIDEO_WINDOW_POS", sdl_wdw_ctr[] = "SDL_VIDEO_CENTERED=1";
    putenv(sdl_wdw_pos);
    putenv(sdl_wdw_ctr);

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    window_width = 640;
    window_height = 480;
    window_title = "Avatar Main Window";

    SDL_WM_SetCaption(window_title, 0);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

    sdl_pimage = SDL_SetVideoMode(window_width, window_height, SDL_DEPTH, SDL_VIDEO_MODE_OPTIONS);
    if(sdl_pimage == NULL)
        return false;

    return true;
}

void CAvatar::OnCleanup()
{
    if (sdl_pimage)
        SDL_FreeSurface(sdl_pimage);
    SDL_Quit();
}

void CAvatar::OnEvent(SDL_Event * Event)
{

}

void CAvatar::OnLoop()
{

}

void CAvatar::OnRender()
{

}
