/*
    James William Fletcher (github.com/mrbid)
        September 2021

    Abstract:

        Application 16x16 icons are from http://www.forrestwalter.com/icons/

        Just a simple ray tracer that uses spheres.

        Lambertian/Fullbirght shaded with the light source being behind the ray origin (camera)


    Origin:

        This is a C implementation of:
        https://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing&src=0
        
        Which is part of this article:
        https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/how-does-it-work

        Which is written by Jean-Colas Prunier who runs Scratchapixel, more information about Jean can be found here:
        https://www.scratchapixel.com/about.php


    Objective:

        To make a real-time simulation or game using a simple and fast raytracing renderer in C.


    References:

        //

*/
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#include "sdl_extra.h"
#include "icon.h"

#include "vec.h"

#define uint unsigned int

SDL_Rect screen_rect = {0, 0, 640, 480};
SDL_Window *window = NULL;
SDL_Surface* screen = NULL;
char *basedir, *appdir;

SDL_Surface* bb; // backbuffer
SDL_Surface* s_icon;

#define MAX_SPHERES 1
typedef struct {
    float x,y,z,rad,rad2,r,g,b;
} sphere;
sphere sphere_array[MAX_SPHERES] = {0};

float p_fov = 30.f;
float p_width = 0.f;
float p_height = 0.f;
float p_invWidth = 0.f;
float p_invHeight = 0.f;
float p_aspectratio = 0.f;
float p_angle = 0.f;


// load/init assets
void loadAssets(SDL_Surface* screen)
{
    srand(SDL_GetTicks());
    bb = SDL_RGBA32Surface(screen->w, screen->h);
    if(rand()%2 == 0)
        s_icon = surfaceFromData((Uint32*)&icon_image.pixel_data[0], 16, 16);
    else
        s_icon = surfaceFromData((Uint32*)&icon_image1.pixel_data[0], 16, 16);

    sphere_array[0] = (sphere){0.f,0.f,-115.f,33.0f,33.0f*33.0f,1.f,0.3f,0.3f};
}

// helper functions
float fMax(float a, float b){return a > b ? a : b;}
float fMin(float a, float b){return a < b ? a : b;}

// check intersection with a sphere
int intersect(const vec rayorig, const vec raydir, float* t0, float* t1, const sphere* s)
{
    vec l;
    vSub(&l, (vec){s->x, s->y, s->z}, rayorig);
    const float tca = vDot(l, raydir);
    if(tca < 0){return 0;}
    const float d2 = vDot(l, l) - tca * tca;
    if(d2 > s->rad2){return 0;}
    const float thc = sqrtps(s->rad2 - d2);
    *t0 = tca - thc;
    *t1 = tca + thc;
    return 1;
}

// return lamberitan colour of sphere intersection
Uint32 lambert_trace(const vec rayorig, const vec raydir)
{
    // find intersection with a sphere
    float tnear = FLOAT_MAX;
    sphere* sphr = NULL;
    for(uint i = 0; i < MAX_SPHERES; i++)
    {
        float t0 = FLOAT_MAX, t1 = FLOAT_MAX;
        if(intersect(rayorig, raydir, &t0, &t1, &sphere_array[i]))
        {
            if(t0 < 0){t0 = t1;}
            if(t0 < tnear)
            {
                tnear = t0;
                sphr = &sphere_array[i];
            }
        }
    }

    // if there's no intersection return black
    if(sphr == NULL)
        return 0xFF00FF00;

    // hit position
    vec phit;
    vAdd(&phit, rayorig, raydir);
    vMulS(&phit, phit, tnear);

    // hit normal vector
    vec nhit;
    vSub(&nhit, phit, (vec){sphr->x, sphr->y, sphr->z});
    vNorm(&nhit);

    // light is always behind ray origin like a flashlight
    //const vec lightpos = (vec){0.0f, 0.0f, 130.f};
    vec lightpos;
    vAdd(&lightpos, rayorig, (vec){0.0f, 0.0f, 130.f});
    vec lightDirection;
    vSub(&lightDirection, lightpos, phit);
    vNorm(&lightDirection);

    // return surface colour lambertian
    const float angle = fMax(vDot(nhit, lightDirection), 0.f);
    return SDL_MapRGB(bb->format, (Uint8)(sphr->r*angle*255.f), (Uint8)(sphr->g*angle*255.f), (Uint8)(sphr->b*angle*255.f));
}

// return fullbright colour of sphere intersection
Uint32 fullbright_trace(const vec rayorig, const vec raydir)
{
    // find intersection with a sphere
    float tnear = FLOAT_MAX;
    sphere* sphr = NULL;
    for(uint i = 0; i < MAX_SPHERES; i++)
    {
        float t0 = FLOAT_MAX, t1 = FLOAT_MAX;
        if(intersect(rayorig, raydir, &t0, &t1, &sphere_array[i]))
        {
            if(t0 < 0){t0 = t1;}
            if(t0 < tnear)
            {
                tnear = t0;
                sphr = &sphere_array[i];
            }
        }
    }

    // if there's no intersection return black
    if(sphr == NULL)
        return 0xFF00FF00;

    // return surface colour
    //printf("a0: %.2f %.2f %.2f\n", sphr->r*255.f, sphr->g*255.f, sphr->b*255.f);
    return SDL_MapRGB(bb->format, (Uint8)(sphr->r*255.f), (Uint8)(sphr->g*255.f), (Uint8)(sphr->b*255.f));
}

// set perspective
void setPerspective(float fov)
{
    p_fov = fov;
    p_width = (float)screen_rect.w;
    p_height = (float)screen_rect.h;

    p_invWidth = 1.f / p_width;
    p_invHeight = 1.f / p_height;

    p_aspectratio = p_width / p_height;
    p_angle = tan(PI * 0.5f * p_fov / 180.f);
}

// render scene
void render()
{
    // cast a ray per pixel
    Uint32* p = (Uint32*)bb->pixels;
    for(Uint32 y = 0; y < screen->h; ++y)
    {
        for(Uint32 x = 0; x < screen->w; ++x, ++p)
        {
            const float xx = (2.f * ((x + 0.5f) * p_invWidth) - 1.f) * p_angle * p_aspectratio;
            const float yy = (1.f - 2.f * ((y + 0.5f) * p_invHeight)) * p_angle;
            vec raydir = (vec){xx, yy, -1.f, 0.f};
            vNorm(&raydir);
            *p = lambert_trace((vec){0.f, 0.f, 0.f}, raydir);
        } 
    } 

    // blit to screen
    SDL_BlitSurface(bb, NULL, screen, NULL);
    SDL_UpdateWindowSurface(window);
}

// program entry-point
int main(int argc, char *args[])
{
    // init sdl
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "ERROR: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    // create window
    window = SDL_CreateWindow("VoxRay", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_rect.w, screen_rect.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        fprintf(stderr, "ERROR: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }

    // create screen
    screen = SDL_GetWindowSurface(window);

    // get app dir
    // basedir = SDL_GetBasePath();
    // appdir = SDL_GetPrefPath("voxdsp", "ray");
    // printf("%s\n", basedir);
    // printf("%s\n", appdir);

    // load assets
    loadAssets(screen);

    // set icon
    SDL_SetWindowIcon(window, s_icon);

    // set perspective
    setPerspective(30.f);

    // first render
    render();

    // event loop
    while(1)
    {
        SDL_Event event;
        while(SDL_WaitEvent(&event))
        {
            switch(event.type)
            {
                case SDL_WINDOWEVENT:
                {
                    if(event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        // resize screen rect
                        screen_rect.w = event.window.data1;
                        screen_rect.h = event.window.data2;

                        // free screen and re-allocate
                        SDL_FreeSurface(screen);
                        screen = SDL_GetWindowSurface(window);

                        // free back buffer and re-allocate
                        SDL_FreeSurface(bb);
                        bb = SDL_RGBA32Surface(screen->w, screen->h);

                        // set perspective
                        setPerspective(30.f);

                        // update at 20 fps
                        static Uint32 lr = 0;
                        if(SDL_GetTicks() > lr)
                        {
                            render();
                            lr = SDL_GetTicks() + 50;
                        }
                        break;
                    }

                    // update at 20 fps
                    static Uint32 lr = 0;
                    if(SDL_GetTicks() > lr)
                    {
                        render();
                        lr = SDL_GetTicks() + 50;
                    }
                }
                break;

                case SDL_MOUSEMOTION:
                {
                    //render();
                }
                break;

                case SDL_KEYDOWN:
                {
                    //
                }
                break;

                case SDL_MOUSEBUTTONUP:
                {
                    //
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    //
                }
                break;
                
                case SDL_QUIT:
                {
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    exit(0);
                }
                break;
            }
        }
    }

    //Done.
    return 0;
}

