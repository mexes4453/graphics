#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <iostream>

/* for documentation - https://docs.gl */
/* MACRO DEFINITIONS */
#define COL_BLUE "\033[0;34m"
#define COL_RED "\033[0;31m"
#define COL_GREEN "\033[0;32m"
#define COL_YELLOW "\033[0;33m"
#define COL_MAGENTA "\033[0;35m"
#define COL_DEFAULT "\033[0m"

#define UTILS_ASSERT(condition, msg)                               \
    if (!(condition))                                              \
    {                                                              \
        std::cerr << COL_RED << (msg) << COL_DEFAULT << std::endl; \
        exit(EXIT_FAILURE);                                        \
    }

#define COUT std::cout
#define CERR std::cerr
#define ENDL std::endl
#define APP_TITLE ("OpenGL App")
#define WIN_HEIGHT (800)
#define WIN_WIDTH (600)

/* Global variables */
SDL_Window   *g_pAppWin = nullptr;
SDL_GLContext g_pOpenGlContext = nullptr;
bool          gQuit = false;

/* Function definitions */
void getOpenGlVersionInfo(void)
{
    COUT << "VENDOR: " << glGetString(GL_VENDOR) << ENDL;
    COUT << "RENDERER: " << glGetString(GL_RENDERER) << ENDL;
    COUT << "VERSION: " << glGetString(GL_VERSION) << ENDL;
    COUT << "SHADING LANG: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << ENDL;
}



void init(void)
{
    int retCode = 0;

    /* Initialise the video subsystem for the app window */
    retCode = SDL_Init(SDL_INIT_VIDEO);
    UTILS_ASSERT((retCode >= 0), "Error! SDL_Init failed")

    /* Set attribute of opengl context */
    /* Select version 4.1 */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    /* Disable all deprecated function and use only current features */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    /* Enable the use of double buffer for smooth rendering transition */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    /* Set depth size to determine object distance from view or camera */
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);



    /* Create the SDL window */
    g_pAppWin = SDL_CreateWindow(APP_TITLE,
                                 0, 0, WIN_WIDTH, WIN_HEIGHT,
                                 SDL_WINDOW_OPENGL);
    UTILS_ASSERT((g_pAppWin != nullptr), "Error! SDL_CreateWindow failed")

    /* Create an open gl context */
    g_pOpenGlContext = SDL_GL_CreateContext(g_pAppWin);
    UTILS_ASSERT((g_pOpenGlContext != nullptr), "Error! SDL_GL_CreateContext failed")
    

    /* Show openGL version information */
    getOpenGlVersionInfo();
}


void handleInputs(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            std::cout << "Exiting application..." << std::endl;
            gQuit = true;
        }
    }

}

void mainloop(void)
{
    while (!gQuit)
    {
        handleInputs();

        /* predraw */
        /* update - draw  */
    }
}

void cleanup(void)
{
    SDL_DestroyWindow(g_pAppWin);
    SDL_Quit();
}

int main(void)
{
    init();
    mainloop();
    cleanup();

    return (0);
}
