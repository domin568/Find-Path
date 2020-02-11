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
#include <iomanip>

#include "headers/graph.h"


SDL_Surface *screen;
SDL_Renderer *sdlRenderer;
SDL_Window *window;
const int width = 800;
const int height = 600;
const char * windowName = "Find Path";

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
void drawConnections (connection * c, int n, uint8_t R, uint8_t G, uint8_t B)
{
    SDL_SetRenderDrawColor(sdlRenderer, R, G, B, SDL_ALPHA_OPAQUE);
    for (int i = 0 ; i < n ; i++)
    {
        uint32_t x1 = c[i].s.getPosition().x;
        uint32_t y1 = c[i].s.getPosition().y;
        uint32_t x2 = c[i].t.getPosition().x;
        uint32_t y2 = c[i].t.getPosition().y;

        SDL_RenderDrawLine(sdlRenderer, x1,y1,x2,y2);
        
    }
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
connection * createConnectionFromPath (std::vector <Node> nodes)
{
    connection * c = new connection [nodes.size()-1]; 
    for (int i = 0; i < nodes.size()-1; i++)
    {
        c[i].s = nodes[i];
        c[i].t = nodes[i+1];
    }
    return c;
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
    
    if (argc < 3)
    {
        std::cout << "[!] Usage: ./find_path <node ID> <node ID> " << std::endl;
        return 1; 
    }
    
    uint32_t nodeFrom = atoi (argv [1]); // that's enough for this application
    uint32_t nodeTo = atoi (argv [2]);
    
    if (initializeSDL())
    {
        return 2;
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
    Graph g (10,14,nodes,c);
    
    std::vector <Node> p = g.path (nodes[nodeFrom],nodes[nodeTo],nodes);

    connection * shortestConnections = createConnectionFromPath (p);
    uint32_t szShortestConnections = p.size()-1;

    double pathDistance = 0;
    calcDistances (shortestConnections,szShortestConnections);
    for (int i = 0 ; i < szShortestConnections; i++)
    {
         pathDistance += shortestConnections[i].distance;
    }

    std::cout << "[*] Distance of path : " << std::setprecision(4) << pathDistance << std::endl;

    SDL_Event event;
    bool done = false;
    TTF_Font* sans = TTF_OpenFont("Sans.ttf", 12); //this opens a font style and sets a size

    while(!done) 
    {
        drawConnections (c,14,255,0,0);
        drawConnections (shortestConnections,szShortestConnections,0,255,255);
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

                    }
                }
            }
        }  
    }
    delete [] shortestConnections;
    return 0;
}
