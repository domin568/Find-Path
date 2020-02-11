#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#include <SDL.h>
#include <SDL_ttf.h>

#include <math.h>
#include <time.h>
#include <iostream>
#include <random>
#include <cmath>
#include <sstream>

#include "headers/graph.h"


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

bool initializeSDL ()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    TTF_Init();
    window = SDL_CreateWindow(windowName, 100, 100, width, height, SDL_WINDOW_SHOWN);
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
uint32_t randNum (uint32_t from, uint32_t to)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(from,to); 
    return dist(rng);
}

SDL_Point * randomPoints (uint32_t nodes)
{
    SDL_Point * points = new SDL_Point[nodes];
    for (int i = 0 ; i < nodes; i++)
    {
        points[i].x = randNum(0,width*2);
        points[i].y = randNum(0,height*2);
    }

    return points;
}
double calcDistance (Vector2d point1, Vector2d point2)
{
    return sqrt (pow(point1.x - point2.x,2) + pow (point1.y - point2.y,2));
}
void calcDistances (connection c [], int n)
{
    for (int i = 0; i < n; i++)
    {
        c[i].distance = calcDistance(c[i].s.getPosition(),c[i].t.getPosition());
    }
}
void initializeNodes (Node * nodes)
{
    Vector2d * positions = new Vector2d [10];
    positions[0] = {33,558};
    positions[1] = {383,463};
    positions[2] = {236,289};
    positions[3] = {153,200};
    positions[4] = {103,81};
    positions[5] = {388,70};
    positions[6] = {636,78};
    positions[7] = {364,175};
    positions[8] = {560,180};
    positions[9] = {503,297};

    for (int i = 0 ; i < 10; i++)
    {
        nodes[i].setID (i);
        nodes[i].setPosition (positions[i]);
    }
}
void createGraph (Node * nodes, connection * c)
{
    Graph g (10,14,nodes,c);
}
void drawConnections (connection * c, int n, uint8_t R, uint8_t G, uint8_t B)
{
    //SDL_Point * points = new SDL_Point [n * 2];
    SDL_SetRenderDrawColor(sdlRenderer, R, G, B, SDL_ALPHA_OPAQUE);
    for (int i = 0 ; i < n ; i++)
    {
        uint32_t x1 = c[i].s.getPosition().x;
        uint32_t y1 = c[i].s.getPosition().y;
        uint32_t x2 = c[i].t.getPosition().x;
        uint32_t y2 = c[i].t.getPosition().y;
        /*
        points[i*2].x = c[i].s.getPosition().x;
        points[i*2].y = c[i].s.getPosition().y;
        points[i*2 + 1].x = c[i + 1].t.getPosition().x;
        points[i*2 + 1].y = c[i + 1].t.getPosition().y;
        */
        SDL_RenderDrawLine(sdlRenderer, x1,y1,x2,y2);
        
    }

    //delete [] points;
}
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
void drawDistances (connection * c, int n, uint8_t R, uint8_t G, uint8_t B, TTF_Font * font)
{
    SDL_Color color = {R, G, B};

    for (int i = 0 ; i < n ; i++)
    {
        uint32_t midX = (abs(c[i].s.getPosition().x + c[i].t.getPosition().x)) / 2;
        uint32_t midY = (abs(c[i].s.getPosition().y + c[i].t.getPosition().y)) / 2;

        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, to_string_with_precision(c[i].distance,2).c_str(), color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
        SDL_Texture* message = SDL_CreateTextureFromSurface(sdlRenderer, surfaceMessage); //now you can convert it into a texture

        SDL_Rect messageRect; //create a rect
        messageRect.x = midX;  //controls the rect's x coordinate 
        messageRect.y = midY; // controls the rect's y coordinte
        messageRect.w = 60; // controls the width of the rect
        messageRect.h = 30; // controls the height of the rect

        SDL_RenderCopy(sdlRenderer, message, NULL, &messageRect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    }
}
void drawIDs (Node * nodes, int n, uint8_t R, uint8_t G, uint8_t B, TTF_Font * font)
{
    SDL_Color color = {R, G, B};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    for (int i = 0 ; i < n ; i++)
    {
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, std::to_string(nodes[i].getID()).c_str(), color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
        SDL_Texture* message = SDL_CreateTextureFromSurface(sdlRenderer, surfaceMessage); //now you can convert it into a texture

        SDL_Rect messageRect; //create a rect
        messageRect.x = nodes[i].getPosition().x;  //controls the rect's x coordinate 
        messageRect.y = nodes[i].getPosition().y; // controls the rect's y coordinte
        messageRect.w = 20; // controls the width of the rect
        messageRect.h = 50; // controls the height of the rect

        SDL_RenderCopy(sdlRenderer, message, NULL, &messageRect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    }
}
void drawNodes (Node * nodes, int n, uint8_t R, uint8_t G, uint8_t B)
{
    const int rectSize = 15;
    SDL_SetRenderDrawColor(sdlRenderer, R, G, B, SDL_ALPHA_OPAQUE);
    for (int i = 0 ; i < n; i++)
    {
        SDL_Rect rect = {(int)nodes[i].getPosition().x-rectSize/2,(int)nodes[i].getPosition().y - rectSize/2,rectSize,rectSize};
        SDL_RenderDrawRect(sdlRenderer,&rect);
    }
}

int main (int argc, char ** argv) 
{
    if (initializeSDL())
    {
        return -1;
    }

    Node * nodes = new Node [10];
    initializeNodes (nodes);


    connection c [] = 
    {
        {nodes[0],nodes[1]}, 
        {nodes[1],nodes[9]}, 
        {nodes[1],nodes[2]}, 
        {nodes[2],nodes[3]}, 
        {nodes[2],nodes[9]}, 
        {nodes[9],nodes[7]}, 
        {nodes[9],nodes[8]}, 
        {nodes[7],nodes[3]}, 
        {nodes[7],nodes[5]}, 
        {nodes[7],nodes[6]}, 
        {nodes[4],nodes[5]}, 
        {nodes[3],nodes[4]}, 
        {nodes[5],nodes[6]}, 
        {nodes[6],nodes[8]}
    };
    calcDistances (c,14);
    createGraph (nodes,c);

    SDL_Event event;
    bool done = false;

    TTF_Font* sans = TTF_OpenFont("Sans.ttf", 12); //this opens a font style and sets a size

    while(!done) 
    {
        drawConnections (c,14,255,0,0);
        drawNodes (nodes,10,0,255,0);
        drawIDs(nodes,10,255,255,255,sans);
        drawDistances(c,14,255,255,0,sans);
        SDL_RenderPresent(sdlRenderer);
        while(SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
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
                        //line (pixels,rand()%width, rand()%height, rand()%width, rand()%height, 100+rand()%155, 100+rand()%155, 100+rand()%155);
                    }
                }
            }
        }
        //SDL_RenderClear(sdlRenderer);
        //SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
        
    }

    return 0;
}
