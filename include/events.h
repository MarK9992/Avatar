#ifndef _EVENTS_H_
#define _EVENTS_H_

#include <SDL.h>

class CEvent
{
public:
    CEvent();
    virtual ~CEvent();

    virtual void OnEvent(SDL_Event* Event);
    virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    virtual void OnResize(int w, int h);
    virtual void OnExpose();
    virtual void OnExit();
    virtual void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
    virtual void OnMouseWheel(bool up, bool down);
};

#endif
