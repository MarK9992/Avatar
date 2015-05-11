#include "avatar.h"
#include <iostream>
#include "gl_objects.h"
#include <GL/glu.h>

using namespace std;

#define SDL_VIDEO_MODE_OPTIONS (SDL_RESIZABLE | SDL_HWSURFACE | SDL_OPENGL)
#define SDL_DEPTH 32

#define SCENE_ROTATION_STEP 5
#define CAMERA_Z_OFFSET 2
#define CAMERA_TRANSLATION_STEP 0.1

#define RDR_FRAME_LENGTH 1
#define RDR_CUBE_HALF_SIDE 0.5

CAvatar::CAvatar()
{
    should_be_running = false;
    //debug git
    // indique si l'avatar doit être (re)modélisé
    needs_rendering = true;

    // dimensions et titre de la fenêtre
    window_width = 0;
    window_height = 0;
    window_title = NULL;

    // la surface framebuffer correspondant à la fenêtre
    sdl_pimage = NULL;

    // l'ID de la texture
    texture = 0;
    // ses dimensions

    // coordonnées de l'origine
    world_origin_x = 0;
    world_origin_y = 0;
    world_origin_z = 0;

    InitSceneConstants();
}

CAvatar::~CAvatar() {}

void CAvatar::InitSceneConstants()
{
    world_rx = 0;
    world_ry = 0;

    camera_min_tz = world_origin_z + CAMERA_Z_OFFSET;
    camera_tx = world_origin_x;
    camera_ty = world_origin_y;
    camera_tz = camera_min_tz;
}

void CAvatar::SetPerspectiveProjectionMatrix() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera_fovy, camera_aspect_ratio, camera_min_z, camera_max_z);
}

bool CAvatar::OnInit(bool mode)
{
    char sdl_wdw_pos[] = "SDL_VIDEO_WINDOW_POS", sdl_wdw_ctr[] = "SDL_VIDEO_CENTERED=1";

    putenv(sdl_wdw_pos);
    putenv(sdl_wdw_ctr);

    sensor_mode = mode;

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

    /*
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
    */

    //SDL_GL_SetAttribute(SDL_HWSURFACE, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    sdl_pimage = SDL_SetVideoMode(window_width, window_height, SDL_DEPTH, SDL_VIDEO_MODE_OPTIONS);
    if(sdl_pimage == NULL)
        return false;

    // initialisation d'OpenGL

    glClearColor(0,0,0,0);
    glViewport(0,0, window_width, window_height);

    camera_aspect_ratio = ((float)window_width) / ((float)window_height);
    camera_min_z = 0.1;
    camera_max_z = 10;
    camera_fovy = 60;

    if(!sensor_mode)
    {
        SetPerspectiveProjectionMatrix();

        // initialisation de la texture
        SDL_Surface *temp;
        if((temp = SDL_LoadBMP("../images/stones.bmp")) == NULL)
        {
            cerr << "unable to load texture" << endl;
        }
        glEnable(GL_TEXTURE_2D);
        texture = Load2DTexture(temp->w, temp->h, temp->format->BytesPerPixel, temp->pixels);

        // initialisation de la lumière
        glEnable(GL_LIGHTING);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_LIGHT0);

        // Définissez les paramètres de la source 0
        GLfloat params[4] = {1, -1, 1, 0};
        glLightfv(GL_LIGHT0, GL_POSITION, params);
    }
    else
    {
        SetOrthoProjectionMatrix();
        glEnable(GL_TEXTURE_2D);
        if(!sensor.OnInit(true))
        {
            return false;
        }
    }

    return true;
}

int CAvatar::OnExecute(bool mode)
{
    if(OnInit(mode) == false) {
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

void CAvatar::OnCleanup()
{
    if (sdl_pimage)
        SDL_FreeSurface(sdl_pimage);
    SDL_Quit();
}

void CAvatar::OnLoop()
{

}

void CAvatar::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    GLfloat scaling[] = {1, 0,  0, 0,
                         0, 1.5,0, 0,
                         0, 0,  1, 0,
                         0, 0,  0, 1};

    glMatrixMode(GL_MODELVIEW);
    // on initialise à l'identité
    glLoadIdentity();
    // on se déplace à la position de la caméra
    glTranslatef(-camera_tx, -camera_ty, -camera_tz);
    // on effectue la rotation selon x
    glRotatef(world_rx, 1, 0, 0);
    // on effectue la rotation selon y
    glRotatef(world_ry, 0, 1, 0);
    // on applique l'échelle
    glMultMatrixf(scaling);

    // on appelle  la methode on fontion du mode choisi
    if(sensor_mode)
    {
        CAvatar::DrawSensor();
    }
    else
    {
        CAvatar::DrawDemo();
    }

    SDL_GL_SwapBuffers();
}

void CAvatar::OnEvent(SDL_Event * Event)
{
    CEvent::OnEvent(Event); // voir tutoriel en ligne
}

void CAvatar::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicod)
{
    switch (sym)
    {
        case SDLK_ESCAPE:
            OnExit();
            break;

        case SDLK_SPACE:
            InitSceneConstants();
            needs_rendering = true;
            break;

        case SDLK_LEFT:
            if (mod && KMOD_SHIFT)
            {
                world_ry -= SCENE_ROTATION_STEP;
                if(world_ry < 0)
                {
                    world_ry += 360;
                }
            }
            else translate(-CAMERA_TRANSLATION_STEP, 0);
            needs_rendering =true;
            break;

        case SDLK_RIGHT:
            if (mod && KMOD_SHIFT)
            {
                world_ry += SCENE_ROTATION_STEP;
                if(world_ry > 360)
                {
                    world_ry -= 360;
                }
            }
            else translate(CAMERA_TRANSLATION_STEP, 0);
            needs_rendering =true;
            break;

        case SDLK_DOWN:
            if (mod && KMOD_SHIFT)
            {
                world_rx -= SCENE_ROTATION_STEP;
                if(world_rx < 0)
                {
                    world_rx += 360;
                }
            }
            else translate(0, -CAMERA_TRANSLATION_STEP);
            needs_rendering =true;
            break;

        case SDLK_UP:
            if (mod && KMOD_SHIFT)
            {
                world_rx += SCENE_ROTATION_STEP;
                if(world_rx > 360)
                {
                    world_rx -= 360;
                }
            }
            else translate(0, CAMERA_TRANSLATION_STEP);
            needs_rendering =true;
            break;

        case SDLK_q:
            zoom(-CAMERA_TRANSLATION_STEP);
            needs_rendering = true;
            break;

        case SDLK_a:
            zoom(CAMERA_TRANSLATION_STEP);
            needs_rendering = true;
            break;

        case SDLK_c:
            SwitchStream(color_stream);
            break;

        case SDLK_p:
            SwitchStream(depth_stream);
            break;
    }
}

void CAvatar::OnResize(int w,int h)
{
    window_width = w;
    window_height = h;

    SDL_FreeSurface(sdl_pimage);
    sdl_pimage = SDL_SetVideoMode(window_height, window_height, SDL_DEPTH, SDL_VIDEO_MODE_OPTIONS);

    glViewport(0, 0, window_width, window_height);

    camera_aspect_ratio = ((float)window_height) / ((float)window_height);
    SetPerspectiveProjectionMatrix();

    needs_rendering = true;
}

/**
 * @brief CAvatar::OnExpose
 * Sets needs_rendering to true in order to render the avatar at the next loop.
 */
void CAvatar::OnExpose()
{
    needs_rendering = true;
}

void CAvatar::OnExit()
{
    should_be_running = false;
}

void CAvatar::translate(double relX, double relY)
{
    camera_tx += relX;
    camera_ty += relY;
}

void CAvatar::rotate(int relX, int relY)
{
    // pour éviter de pivoter à plus de 90° l'avatar et donc de ne pas inverser la rotation selon y
    if(!(world_rx < -89 && relY < 0 || world_rx > 89 && relY > 0))
    {
        world_rx += relY;
    }
    world_ry += relX;
}

void CAvatar::zoom(double relY)
{
    camera_tz += relY;
    if (camera_tz < camera_min_tz)
    {
        camera_tz = camera_min_tz;
    }
}

void CAvatar::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle)
{
    if(Left) translate(-((double) relX) / 100, ((double) relY) / 100);
    else if(Right) rotate(relX, relY);
    needs_rendering = true;
}

void CAvatar::OnMouseWheel(bool up, bool down)
{
    if(up) zoom(CAMERA_TRANSLATION_STEP);
    else zoom(-CAMERA_TRANSLATION_STEP);
    needs_rendering = true;
}



// la methode contient le ancien code de OnRender
void CAvatar::DrawDemo()
{
    if(!needs_rendering)
        return;
    needs_rendering = false;

    // Définition des propriétés du matériau de l'objet
    GLfloat paramsDiffuse[4] = {0.8, 0.8, 0.8, 1.0};
    GLfloat paramsSpecular[4] = {0, 0, 0, 1};
    GLfloat paramsEmission[4] = {0.5, 0.5, 0.5, 1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, paramsDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, paramsSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
    glMaterialfv(GL_FRONT, GL_EMISSION, paramsEmission);

    // on dessine nos objets
    DrawFrame(world_origin_x, world_origin_y, world_origin_z, RDR_FRAME_LENGTH);
    //DrawCube(world_origin_x, world_origin_y, world_origin_z, RDR_CUBE_HALF_SIDE);
    DrawCubeWithTexture(world_origin_x, world_origin_y, world_origin_z, RDR_CUBE_HALF_SIDE, texture);
}

void CAvatar::DrawSensor()
{
    openni::VideoFrameRef m_colorFrame;

    sensor.m_colorStream.readFrame(&m_colorFrame);
    if (! m_colorFrame.isValid())
        return;

    const openni::RGB888Pixel* pImage = (const openni::RGB888Pixel*) m_colorFrame.getData();

    if(sensor.active_stream == color_stream)
    {
        texture = Load2DTexture(m_colorFrame.getWidth(), m_colorFrame.getHeight(), 3, pImage);
        FillWindowWithTexture(texture);
        glDeleteTextures(1, &texture);
    }
    else
    {
       openni::VideoFrameRef m_depthFrame;

       sensor.m_depthStream.readFrame(&m_depthFrame);
       if (! m_depthFrame.isValid())
           return;

       const openni::DepthPixel* pDepth = (const openni::DepthPixel*) m_depthFrame.getData();

       // n'oubliez pas d'effacer les buffers couleur et profondeur d'OpenGL
       int width = m_depthFrame.getWidth();
       int height = m_depthFrame.getHeight();
       float pWorldX, pWorldY, pWorldZ;
       // définissez la matrice ModeLView comme d'habiture (fait plus haut)

       glEnable(GL_DEPTH_TEST);
       glPointSize(2);
       glBegin(GL_POINTS);

       for (int y = 0; y < height; y++) {
           for (int x = 0; x < width; x++) {
               if ((x % 2 == 0) && (y % 2 == 0) && (*pDepth != 0)) {
                   openni::CoordinateConverter::convertDepthToWorld(sensor.m_depthStream,
                                                                    x, y, *pDepth, &pWorldX,
                                                                    &pWorldY, &pWorldZ);
                   glColor3f(1,1,1);
                   glVertex3f(pWorldX / 1000.0, pWorldY / 1000.0, pWorldZ / 1000.0);
               }
           }
           pDepth++;
           pImage++;
       }
    }
}

void CAvatar::SetOrthoProjectionMatrix()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -10, 10);
}

/**
 * @brief SwitchStream
 * change le stream actif pour celui passe en parametre
 */
void CAvatar::SwitchStream(EActiveStream stream)
{
    InitSceneConstants();
    sensor.SwitchActiveStream(stream);
    if (sensor.active_stream == color_stream)
        SetOrthoProjectionMatrix();
    else
        SetPerspectiveProjectionMatrix();
}
