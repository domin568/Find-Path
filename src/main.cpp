#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#include <SDL.h>
#include <math.h>
#include <time.h>
#include <iostream>

SDL_Surface *screen;
SDL_Renderer *sdlRenderer;
SDL_Window *window;
const int width = 800;
const int height = 600;
const char * windowName = "Find Path (Mac OS)";

void line (uint32_t * pixels, int x1, int y1, int x2, int y2, uint8_t R, uint8_t G, uint8_t B);

void line (uint32_t * pixels, int x1, int y1, int x2, int y2, uint8_t R, uint8_t G, uint8_t B) 
{
    uint32_t color = 0xff000000 | (uint32_t) R << 16 | (uint32_t) G << 8 | (uint32_t) B;
    std::cout << "color  = " << std::hex << color << std::endl;
    pixels[y1 * width + x1] = color;
    pixels[y2 * width + x2] = color;
    
     // zmienne pomocnicze
     int d, dx, dy, ai, bi, xi, yi;
     int x = x1, y = y1;
     // ustalenie kierunku rysowania
     if (x1 < x2)
     {
         xi = 1;
         dx = x2 - x1;
     }
     else
     {
         xi = -1;
         dx = x1 - x2;
     }
     // ustalenie kierunku rysowania
     if (y1 < y2)
     {
         yi = 1;
         dy = y2 - y1;
     }
     else
     {
         yi = -1;
         dy = y1 - y2;
     }
     // pierwszy piksel
     pixels[y * width + x] = color;
     // oś wiodąca OX
     if (dx > dy)
     {
         ai = (dy - dx) * 2;
         bi = dy * 2;
         d = bi - dx;
         // pętla po kolejnych x
         while (x != x2)
         {
             // test współczynnika
             if (d >= 0)
             {
                 x += xi;
                 y += yi;
                 d += ai;
             }
             else
             {
                 d += bi;
                 x += xi;
             }
             pixels[y * width + x] = color;
         }
     }
     // oś wiodąca OY
     else
     {
         ai = ( dx - dy ) * 2;
         bi = dx * 2;
         d = bi - dy;
         // pętla po kolejnych y
         while (y != y2)
         {
             // test współczynnika
             if (d >= 0)
             {
                 x += xi;
                 y += yi;
                 d += ai;
             }
             else
             {
                 d += bi;
                 y += yi;
             }
             pixels[y * width + x] = color;
         }
     }
}

void cleanScreen(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderPresent(sdlRenderer);
}
bool initializeSDL ()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow(windowName, 100, 100, width, height, SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
    if(window == nullptr) 
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(sdlRenderer == nullptr) 
    {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    return 0;
}
int main (int argc, char ** argv) 
{
    if (initializeSDL())
    {
        return -1;
    }

    SDL_Texture * texture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);

    uint32_t * pixels = new Uint32[width * height];

    memset(pixels, 255, width * height * sizeof(uint32_t));

    SDL_Event event;
    bool done = false;
    bool leftMouseButtonDown = false;

    while(!done) 
    {
        SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(uint32_t));
        while(SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    delete[] pixels;
                    SDL_DestroyTexture(texture);
                    SDL_DestroyRenderer(sdlRenderer);
                    done = true;
                    break;
                }
                
                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        done = true;
                    }
                    if (event.key.keysym.sym == SDLK_l)
                    {
                        line (pixels,rand()%width, rand()%height, rand()%width, rand()%height, 100+rand()%155, 100+rand()%155, 100+rand()%155);
                    }
                    if (event.key.keysym.sym == SDLK_b)
                    {
                        cleanScreen(0, 0, 10);
                        break;
                    }
                }
                case SDL_MOUSEBUTTONUP:
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        leftMouseButtonDown = false;
                        break;
                    }
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        leftMouseButtonDown = true;
                    }
                }
                case SDL_MOUSEMOTION:
                {
                    if (leftMouseButtonDown)
                    {
                        int mouseX  = event.motion.x;
                        int mouseY = event.motion.y;
                        pixels[mouseY * width + mouseX] = 0;
                    }    
                    break;
                }
                
            }
        }
        SDL_RenderClear(sdlRenderer);
        SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
        SDL_RenderPresent(sdlRenderer);
    }

    return 0;
}
