#include <SDL2/SDL.h>
#include <iostream>

/* Global variables */


void init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL: Error! init failed\n";
        exit(EXIT_FAILURE);
    }
}


void cleanup(void)
{
    SDL_Quit();
}





int main(void)
{
    std::cout << "hello world\n";
    init();
    cleanup();



    return (0);
}
