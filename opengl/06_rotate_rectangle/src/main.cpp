#include <SDL2/SDL.h>
// #include <GL/gl.h>
#include "../inc/glad/glad.h"
#include "../inc/utils.h"
#include "../../../ext/glm-master/glm/vec3.hpp"
#include "../../../ext/glm-master/glm/gtc/matrix_transform.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

/* for documentation - https://docs.gl */
/* MACRO DEFInitIONS */
#define APP_TITLE ("OpenGL App")
#define WIN_HEIGHT (480)
#define WIN_WIDTH (640)

/* Global variables */
SDL_Window *g_pAppWin = nullptr;
SDL_GLContext g_pOpenGlContext = nullptr;
bool g_Quit = false;
GLuint g_vertexArrayObject = 0;  // VAO
GLuint g_vertexBufferObject = 0; // VBO
GLuint g_indexBufferObject = 0;  // IBO
float g_uOffset = -2.4f;          // use of pass offset pos data to uniform var in shaders
float g_uRotate = 0.01f;          // use of pass rotation offset data to uniform var in shaders

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
    const std::vector<GLfloat> vertexData = {
        /* x     y     z   */
        -0.5, -0.5, 0.0, // vertex 1 left bot
        0.5, -0.5, 0.0,  // vertex 2 right bot
        -0.5, 0.5, 0.0,  // vertex 3 left top
        0.5, 0.5, 0.0,   // vertex 4 right top
        1.0, 0.0, 0.0,   // color of vertex 1 red,
        0.0, 1.0, 0.0,   // color of vertex 2 green,
        0.0, 0.0, 1.0,   // color of vertex 3 blue,
        0.0, 0.0, 1.0,   // color of vertex 4 blue,
    };

    const std::vector<GLuint> indexBufferData{2, 0, 1, 3, 2, 1};

    /* Generate the vertex array object */
    glGenVertexArrays(1, &g_vertexArrayObject);
    glBindVertexArray(g_vertexArrayObject);

    /* Generate the buffer object */
    glGenBuffers(1, &g_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObject);

    /* copy the data (vertexData)from cpu to gpu (vertexbufferObject)*/
    glBufferData(GL_ARRAY_BUFFER,
                 vertexData.size() * sizeof(GLfloat),
                 vertexData.data(),
                 GL_STATIC_DRAW);

    /* Set index buffer object (IBO) for specifying data from VBO to GPU */
    glGenBuffers(1, &g_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indexBufferData.size() * sizeof(GLuint),
                 indexBufferData.data(),
                 GL_STATIC_DRAW);

    /* Specify the vertex attribute in the vertex array object */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,        /* Corresponds to the enabledvertexAttribArray */
                          3,        /* Number of components 3 vertices             */
                          GL_FLOAT, /* type : data type - float                    */
                          GL_FALSE, /* Is the data normalised                      */
                          0,        /* stride                                      */
                          NULL);    /* offset                                      */

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,             /* Corresponds to the enabledvertexAttribArray */
                          3,             /* Number of components 3 vertices             */
                          GL_FLOAT,      /* type : data type - float                    */
                          GL_FALSE,      /* Is the data normalised                      */
                          0,             /* stride                                      */
                          (GLvoid *)48); /* offset                              */

    /* Disable used objects VAO */
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
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
    GLint status;
    GLint infoLogLen;
    GLchar *strInfoLog;
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

    /* Check for compilation or linking error*/
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &status); /* get status */
    if (status == GL_FALSE)
    {
        glGetShaderiv(shaderObj, GL_INFO_LOG_LENGTH, &infoLogLen);   /* get err msg len */
        strInfoLog = new GLchar[infoLogLen + 1];                     /* allocate mem    */
        glGetShaderInfoLog(shaderObj, infoLogLen, NULL, strInfoLog); /* get error msg   */
        CERR << "Error! Shader compiler "                            /* print msg       */
             << type << ": " << strInfoLog
             << ENDL;
        delete[] strInfoLog; /* free memory     */
    }
    return (shaderObj);
}

GLuint createShaderProgram(const std::string &vertexShaderSrc,
                           const std::string &fragmentShaderSrc)
{
    GLint status;
    GLint infoLogLen;
    GLchar *strInfoLog;
    GLuint programObj = glCreateProgram();
    GLuint vertex_shader_obj = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fragment_shader_obj = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    glAttachShader(programObj, vertex_shader_obj);
    glAttachShader(programObj, fragment_shader_obj);
    glLinkProgram(programObj);
    glValidateProgram(programObj);

    /* Check for compilation or linking error*/
    glGetProgramiv(programObj, GL_LINK_STATUS, &status); /* get status */
    if (status == GL_FALSE)
    {
        glGetProgramiv(programObj, GL_INFO_LOG_LENGTH, &infoLogLen);   /* get err msg len */
        strInfoLog = new GLchar[infoLogLen + 1];                       /* allocate mem    */
        glGetProgramInfoLog(programObj, infoLogLen, NULL, strInfoLog); /* get error msg   */
        CERR << "Error! Linker : " << strInfoLog << ENDL;              /* print msg       */
        delete[] strInfoLog;                                           /* free memory     */
    }
    /* detach shader objects from program after linking */
    glDetachShader(programObj, vertex_shader_obj);
    glDetachShader(programObj, fragment_shader_obj);
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
    float offset = 0.01f;
    float offsetRotate = 0.11f;
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            std::cout << "\nExiting application..." << std::endl;
            g_Quit = true;
        }
    }

    /* check if keyboard key is pressed */
    const uint8_t *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_UP])
    {
        g_uOffset += offset;
        COUT << "g_uoffset = " << g_uOffset << ENDL;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        g_uOffset -= offset;
        COUT << "g_uoffset = " << g_uOffset << ENDL;
    }
    if (state[SDL_SCANCODE_LEFT])
    {
        g_uRotate -= offsetRotate;
        COUT << "g_uRotate = " << g_uRotate << ENDL;
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        g_uRotate += offsetRotate;
        COUT << "g_uRotate  " << g_uRotate << ENDL;
    }
}

void preDraw()
{
    GLint u_modelOffsetMatrixLocation; /* use to retrieve the location of uniform offset var in shaders */
    GLint u_perspectiveMatrixLocation; /* use to retrieve the location of uniform perspective var in shaders */

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glClearColor(0.f, 0.f, 0.f, 1.f);                   /* set color to use for screen view */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* perform screen clear */

    glUseProgram(g_pipelineShaderProgram);

    /*-----------------------*/
    /* transformation matrix */
    /*-----------------------*/
    /* create the translation matrix to move object */
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, g_uOffset));
    model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.f, 1.0f, 0.f));

    /* retrieve the location of the uniform variable in the shader */
    u_modelOffsetMatrixLocation = glGetUniformLocation(g_pipelineShaderProgram, "u_ModelMatrix");
    UTILS_ASSERT((u_modelOffsetMatrixLocation >= 0), "uniform variable model matrix not found in shader");

    /* send the offset data to the shader */
    glUniformMatrix4fv(u_modelOffsetMatrixLocation, 1, GL_FALSE, &(model[0][0]));

    /*--------------------*/
    /* perspective matrix */
    /*--------------------*/
    /* create the translation matrix to move object */
    glm::mat4 perspective = glm::perspective(glm::radians(20.0f),                  // field of view
                                             (float)WIN_WIDTH / (float)WIN_HEIGHT, // aspect ratio
                                             0.1f,          // near
                                             10.f);         // far

    /* retrieve the location of the uniform variable in the shader */
    u_perspectiveMatrixLocation = glGetUniformLocation(g_pipelineShaderProgram, "u_Perspective");
    UTILS_ASSERT((u_perspectiveMatrixLocation >= 0), "uniform variable perspecitve not found in shader");

    /* send the offset data to the shader */
    glUniformMatrix4fv(u_perspectiveMatrixLocation, 1, GL_FALSE, &(perspective[0][0]));
}

void Draw(void)
{
    glBindVertexArray(g_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObject);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
