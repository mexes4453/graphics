#include <SDL2/SDL.h>
// #include <GL/gl.h>
#include "../inc/glad/glad.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

/* for documentation - https://docs.gl */
/* MACRO DEFInitIONS */
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
#define WIN_HEIGHT (480)
#define WIN_WIDTH (640)

/* Global variables */
SDL_Window *g_pAppWin = nullptr;
SDL_GLContext g_pOpenGlContext = nullptr;
bool g_Quit = false;
GLuint g_vertexArrayObject = 0;     // VAO
GLuint g_vertexBufferObject = 0;    // VBO
GLuint g_pipelineShaderProgram = 0; // handle to shader program

/*-------------------------------------------------------------------------*/
/*                        Function defInitions                             */
/*-------------------------------------------------------------------------*/

void getOpenGlVersionInfo(void)
{
    COUT << "VENDOR: " << glGetString(GL_VENDOR) << ENDL;
    COUT << "RENDERER: " << glGetString(GL_RENDERER) << ENDL;
    COUT << "VERSION: " << glGetString(GL_VERSION) << ENDL;
    COUT << "SHADING LANG: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << ENDL;
}

void VertexSpecification(void)
{
    /* Created and stored on the CPU */
    const std::vector<GLfloat> vertexPos = {
        /* x     y     z   */
        -0.8, -0.8, 0.5, // vertex 1
        0.8, -0.8, 0.0,  // vertex 2
        0.0, 0.8, 0.0    // vertex 3
    };

    /* Generate the vertex array object */
    glGenVertexArrays(1, &g_vertexArrayObject);
    glBindVertexArray(g_vertexArrayObject);

    /* Generate the buffer object */
    glGenBuffers(1, &g_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObject);

    /* copy the data (vertexPos)from cpu to gpu (vertexbufferObject)*/
    glBufferData(GL_ARRAY_BUFFER,
                 vertexPos.size() * sizeof(GLfloat),
                 vertexPos.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,        /* Corresponds to the enabledvertexAttribArray */
                          3,        /* Number of components 3 vertices             */
                          GL_FLOAT, /* type : data type - float                    */
                          GL_FALSE, /* Is the data normalised                      */
                          0,        /* stride                                      */
                          NULL);    /* offset                                      */

    /* Disable used objects */
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

std::string LoadShaderSrcFile(const std::string &file_name)
{
    std::string result = "";
    std::string line = "";
    std::fstream srcFile(file_name.c_str());

    if (srcFile.is_open())
    {
        while (std::getline(srcFile, line))
        {
            result += line + '\n';
        }
        srcFile.close();
    }
    else
    {
        COUT << "file open : failed! " << ENDL;
    }
    return (result);
}



GLuint compileShader(GLuint type, const std::string &sourceCode)
{
    GLuint shaderObj;
    const char *src = sourceCode.c_str();

    /* Create the shader object handle for compilation */
    if (type == GL_VERTEX_SHADER)
    {
        shaderObj = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObj = glCreateShader(GL_FRAGMENT_SHADER);
    }
    /* compile the shader program */
    glShaderSource(shaderObj, 1, &src, nullptr);
    glCompileShader(shaderObj);
    return (shaderObj);
}

GLuint createShaderProgram(const std::string &vertexShader,
                           const std::string &fragmentShader)
{
    GLuint programObj = glCreateProgram();
    GLuint vertex_shader = compileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(programObj, vertex_shader);
    glAttachShader(programObj, fragment_shader);
    glLinkProgram(programObj);
    glValidateProgram(programObj);
    return (programObj);
}

void createGraphicsPipeline(void)
{
    std::string vertexShaderSrc = LoadShaderSrcFile("./shaders/vertex_shader_src.glsl");
    std::string fragmentShaderSrc = LoadShaderSrcFile("./shaders/fragment_shader_src.glsl");

    g_pipelineShaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
}

void Init(void)
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

    /* Initialise the GLAD library - This enables the use of openGL functions */
    UTILS_ASSERT((gladLoadGLLoader(SDL_GL_GetProcAddress) == true),
                 "Error! GLAD loader Init failed")

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
            g_Quit = true;
        }
    }
}

void preDraw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glClearColor(1.f, 1.f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(g_pipelineShaderProgram);
}

void Draw(void)
{
    glBindVertexArray(g_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void mainloop(void)
{
    while (!g_Quit)
    {
        handleInputs();

        /* predraw */
        preDraw();

        /* update - draw  */
        Draw();

        /* update the screen */
        SDL_GL_SwapWindow(g_pAppWin);
    }
}

void cleanup(void)
{
    SDL_DestroyWindow(g_pAppWin);

    /* Delete openGL objects */
    glDeleteBuffers(1, &g_vertexBufferObject);
    glDeleteVertexArrays(1, &g_vertexArrayObject);
    glDeleteProgram(g_pipelineShaderProgram);
    SDL_Quit();
}

int main(void)
{
    Init();
    VertexSpecification();
    createGraphicsPipeline();
    mainloop();
    cleanup();

    return (0);
}
