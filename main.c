/*
    James William Fletcher (james@voxdsp.com)
        September 2021

    Abstract:

        Application 16x16 icons are from http://www.forrestwalter.com/icons/

        Just a simple ray tracer that uses spheres.

        Lambertian/Fullbright shaded with the light source being behind the ray origin (camera)


    Origin:

        This is a C implementation of:
        https://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing&src=0
        
        Which is part of this article:
        https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/how-does-it-work

        Which is written by Jean-Colas Prunier who runs Scratchapixel, more information about Jean can be found here:
        https://www.scratchapixel.com/about.php


    Objective:

        To make a real-time simulation or game using a simple and fast raytraced renderer in C.


    References:

        https://fabiensanglard.net/rayTracing_back_of_business_card/
        https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/how-does-it-work
        https://raytracing.github.io/
        https://github.com/define-private-public/PSRayTracing (very cool project by Benjamin Summerton)


    Notes:

        Initially thought this would be a fun idea, to create a simplified raytraced renderer for a game engine
        that utilises mutliple CPU cores. Until I realised that sending a 3.14mb screen buffer of only 1024x768
        a minimum of 30 times per second was ~100MB of data per second. Which means really raytracing on a CPU
        is only worth while if a render is going to take a long time before you bother trying to display it.
        
        Also I have some aliasing artifact that the original C++ code I transcribed the sphere intersection from
        does not have. No idea what is causing it but it seems to be something to do with the rays diverging more
        as they travel farther from the screen.

        I am not sure if I am going to continue this project because it just seems a little insane to do all this
        just so that I can raytrace some (at best) lambaritan spheres at ~30fps.

        Also the multi-threaded mode does not seem to render anything on my graphics card. But it seems to work fine
        on my integrated Intel Iris XE. ¯\_(ツ)_/¯ not that multi-threading a real-time raytracer has much benefit
        anyway.

        If you are compiling on windows unzip SDL2.zip to the project directory.

        To launch multi-threaded: ./ray multi

        But only single threaded has crude sphere motion to demonstrate the distance aliasing.

*/
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#include "sdl_extra.h"
#include "icon.h"

#include "vec.h"

#define uint unsigned int

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define MAX_SPHERES 32
#define CLEAR_COLOUR 0xFF00FF00

SDL_Rect screen_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Window *window = NULL;
SDL_Surface* screen = NULL;
char *basedir, *appdir;
SDL_Surface* bb; // backbuffer
SDL_Surface* s_icon;
typedef struct {
    float x,y,z,rad,rad2,r,g,b;
    int m;
} sphere;
sphere sphere_array[MAX_SPHERES] = {0};
Uint32 sai = 0;
void addSphere(const vec pos, const float radius, const vec colour, const int rendermode)
{
    sphere_array[sai] = (sphere){pos.x, pos.y, pos.z, radius, radius*radius, colour.x, colour.y, colour.z, rendermode};
    sai++;
}

#define RAYDIR_TABLE_SIZE WINDOW_WIDTH*WINDOW_HEIGHT
vec raydir_table[RAYDIR_TABLE_SIZE];
float p_fov = 0.f;
float p_width = 0.f;
float p_height = 0.f;
float p_invWidth = 0.f;
float p_invHeight = 0.f;
float p_aspectratio = 0.f;
float p_angle = 0.f;
Uint32 numcpu = 0;
Uint32 ppc = 0; // parts per core
Uint32 trc = 1; // thread render count - should be atomic by default as is 4 byte int

// load/init assets
void loadAssets(SDL_Surface* screen)
{
    srand(SDL_GetTicks());
    bb = SDL_RGBA32Surface(screen->w, screen->h);
    if(rand()%2 == 0)
        s_icon = surfaceFromData((Uint32*)&icon_image.pixel_data[0], 16, 16);
    else
        s_icon = surfaceFromData((Uint32*)&icon_image1.pixel_data[0], 16, 16);

    addSphere((vec){0.f,0.f,-115}, 33.f, (vec){1.f,0.3f,0.3f}, 1);
    addSphere((vec){-8.f,0.f,-15}, 3.f, (vec){0.f,1.0f,0.3f}, 3);
    addSphere((vec){0.f,8.f,-15}, 3.f, (vec){0.f,1.0f,0.3f}, 2);
    addSphere((vec){0.f,-8.f,-15}, 3.f, (vec){0.f,1.0f,0.3f}, 0);
}

// helper functions
static inline float fMax(float a, float b){return a > b ? a : b;}
static inline float fMin(float a, float b){return a < b ? a : b;}

// check intersection with a sphere
int intersect(const vec rayorig, const vec raydir, float* t0, const sphere* s)
{
    vec l;
    vSub(&l, (vec){s->x, s->y, s->z}, rayorig);
    const float tca = vDot(l, raydir);
    if(tca < 0.f){return 0;}
    const float d2 = vDot(l, l) - tca * tca;
    if(d2 > s->rad2){return 0;}
    const float thc = sqrtps(s->rad2 - d2);
    *t0 = tca - thc;
    return 1;
}

// return colour of sphere intersection
Uint32 trace(const vec rayorig, const vec raydir)
{
    // find intersection with sphere
    float tnear = FLOAT_MAX;
    sphere* sphr = NULL;
    for(uint i = 0; i < sai; i++)
    {
        float t0 = FLOAT_MAX;
        if(intersect(rayorig, raydir, &t0, &sphere_array[i]) == 1)
        {
            if(t0 < tnear)
            {
                tnear = t0;
                sphr = &sphere_array[i];
                break;
            }
        }
    }

    // no intersection? then it is background
    if(sphr == NULL)
    {
        float t = 0.5f*(raydir.y + 1.0f);
        vec c1 = (vec){1.f, 1.f, 1.f};
        vMulS(&c1, c1, 1.f-t);
        vec c2 = (vec){0.5f, 0.7f, 1.f};
        vMulS(&c2, c2, t);
        vAdd(&c1, c1, c2);
        return SDL_MapRGB(bb->format, (Uint8)(c1.x*255.f), (Uint8)(c1.y*255.f), (Uint8)(c1.z*255.f));
        //return CLEAR_COLOUR;
    }

    // shade the sphere
    if(sphr->m == 0) // fullbright
    {
        return SDL_MapRGB(bb->format, (Uint8)(sphr->r*255.f), (Uint8)(sphr->g*255.f), (Uint8)(sphr->b*255.f));
    }
    else if(sphr->m == 1) // lambartian
    {
        // hit position
        vec phit;
        vAdd(&phit, rayorig, raydir);
        vMulS(&phit, phit, tnear);

        // hit normal vector
        vec nhit;
        vSub(&nhit, phit, (vec){sphr->x, sphr->y, sphr->z});
        vNorm(&nhit);

        // light is always behind ray origin like a flashlight
        const vec lightpos = (vec){0.0f, 0.0f, 130.f};
        //vec lightpos;
        //vAdd(&lightpos, rayorig, (vec){0.0f, 0.0f, 130.f});
        vec lightDirection;
        vSub(&lightDirection, lightpos, phit);
        vNorm(&lightDirection);

        // return surface colour lambertian
        const float angle = fMax(vDot(nhit, lightDirection), 0.f);
        return SDL_MapRGB(bb->format, (Uint8)(sphr->r*angle*255.f), (Uint8)(sphr->g*angle*255.f), (Uint8)(sphr->b*angle*255.f));
    }
    else if(sphr->m == 2) // normals
    {
        // hit position
        vec phit;
        vMulS(&phit, raydir, tnear);
        vAdd(&phit, phit, rayorig);
        // vAdd(&phit, rayorig, raydir);
        // vMulS(&phit, phit, tnear);

        // hit normal vector
        vec N;
        vSub(&N, phit, (vec){sphr->x, sphr->y, sphr->z});
        vNorm(&N);

        // return normals as surface colour
        const vec c1 = (vec){0.5f*(N.x+1.f), 0.5f*(N.y+1.f), 0.5f*(N.z+1.f)};
        return SDL_MapRGB(bb->format, (Uint8)(c1.x*255.f), (Uint8)(c1.y*255.f), (Uint8)(c1.z*255.f));
    }
    else if(sphr->m == 3) // funky normals
    {
        // funky
        vec N;
        vAdd(&N, rayorig, raydir);
        vMulS(&N, N, tnear);
        vSub(&N, N, (vec){0.f, 0.f, -1.f});

        // return normals as surface colour
        const vec c1 = (vec){0.5f*(N.x+1.f), 0.5f*(N.y+1.f), 0.5f*(N.z+1.f)};
        return SDL_MapRGB(bb->format, (Uint8)(c1.x*255.f), (Uint8)(c1.y*255.f), (Uint8)(c1.z*255.f));
    }

    // otherwise fullbright
    return SDL_MapRGB(bb->format, (Uint8)(sphr->r*255.f), (Uint8)(sphr->g*255.f), (Uint8)(sphr->b*255.f));
}

// generate ray origin table
void genRayTable()
{
    Uint32 i = 0;
    for(float y = 0; y < screen->h; y += 1.f)
    {
        for(float x = 0; x < screen->w; x += 1.f, ++i)
        {
            const float xx = (2.f * ((x + 0.5f) * p_invWidth) - 1.f) * p_angle * p_aspectratio;
            const float yy = (1.f - 2.f * ((y + 0.5f) * p_invHeight)) * p_angle;
            vec raydir = (vec){xx, yy, -1.f};
            vNorm(&raydir);
            vCopy(&raydir_table[i], raydir);
        }
    }
}

// set perspective
void setPerspective(float fov)
{
    p_fov = fov;
    p_width = (float)screen_rect.w;
    p_height = (float)screen_rect.h;

    p_invWidth = 1.f / p_width;
    p_invHeight = 1.f / p_height;

    p_aspectratio = p_width * p_invHeight;
    p_angle = tan(d2PI * p_fov * 0.005555556156f); // d2PI * p_fov / 180.f
    
    genRayTable();
}

// render thread (multi-threaded)
Uint32 frame_counter = 0;
static int renderThread(void *ptr)
{
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);
    const Uint32 ti = *(int*)ptr;
    printf("Thread Init: %u\n", ti);

    while(1)
    {
        // this is the screen swapper thread
        if(ti == 0)
        {
            // fps counter
            static Uint32 lt = 0;
            if(SDL_GetTicks() > lt)
            {
                printf("FPS: %u\n", frame_counter/16);
                frame_counter = 0;
                lt = SDL_GetTicks() + 16000;
            }

            // blit to screen if all threads rendered
            if(trc >= numcpu)
            {
                SDL_BlitSurface(bb, NULL, screen, NULL);
                SDL_UpdateWindowSurface(window);
                frame_counter++;
                trc = 1;
            }
        }

        // cast a ray per pixel
        const Uint32 ofs = ti*ppc;
        const Uint32 end = ofs+ppc;
        Uint32* p = (Uint32*)bb->pixels + ofs;
        for(Uint32 i = ofs; i < end; i++, ++p)
            *p = trace((vec){0.f, 0.f, 0.f}, raydir_table[i]);
        trc++;

        SDL_Delay(16);
    }

    return 0;
}

// render thread (single-threaded)
static int singleThread(void *ptr)
{
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);
    while(1)
    {
        // crude motion
        sphere_array[0].z -= 1.6f;
        sphere_array[1].z -= 0.1f;
        sphere_array[2].z -= 0.1f;
        sphere_array[3].z -= 0.1f;

        // cast a ray per pixel
        Uint32* p = (Uint32*)bb->pixels;
        for(Uint32 i = 0; i < RAYDIR_TABLE_SIZE; i++, ++p)
            *p = trace((vec){0.f, 0.f, 0.f}, raydir_table[i]);

        // blit to screen
        SDL_BlitSurface(bb, NULL, screen, NULL);
        SDL_UpdateWindowSurface(window);

        // fps counter
        frame_counter++;
        static Uint32 lt = 0;
        if(SDL_GetTicks() > lt)
        {
            printf("FPS: %u\n", frame_counter/16);
            frame_counter = 0;
            lt = SDL_GetTicks() + 16000;
        }

        SDL_Delay(16);
    }

    return 0;
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
    window = SDL_CreateWindow("VoxRay", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_rect.w, screen_rect.h, SDL_WINDOW_SHOWN);
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
    
    // calc parts per core
    numcpu = SDL_GetCPUCount();
    ppc = RAYDIR_TABLE_SIZE / numcpu;

    // set perspective
    setPerspective(60.f);

    // to thread or not to thread?
    SDL_Thread* single_thread = NULL;
    Uint32 tid[numcpu];
    if(argc == 2 && strcmp(args[1], "multi") == 0)
    {
        for(Uint32 i = 0; i < numcpu; i++)
        {
            tid[i] = i;
            char tn[256];
            sprintf(tn, "renderThread%u", i);
            printf("%s\n", tn);
            SDL_CreateThread(renderThread, tn, (void*)&tid[i]);
        }
    }
    else
    {
        single_thread = SDL_CreateThread(singleThread, "singleThread", (void*)NULL);
    }

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
                    //
                }
                break;

                case SDL_MOUSEMOTION:
                {
                    //
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
                    if(single_thread != NULL)
                        SDL_DetachThread(single_thread);
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

