#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <vector>
#include "../utils/utils.hpp"

int gScreenWidth = 1920;
int gScreenHeight = 1080;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

SDL_Window*   gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

// Stop MainLoop variable
bool done = false;

// VAO
GLuint gVertexArrayObject = 0;

// VBO
GLuint gVertexBufferObject = 0;

// IBO
GLuint gIndexBufferObject = 0;

// Program Object for shaders
GLuint gGraphicsPipelineShaderProgram = 0;

template <typename T>
void CatchErr(T instance, std::string message)
{
    if (!instance)
    {
        std::cerr << "ERROR: " << message << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        exit(1);
    }
}

GLuint CompileShader(GLuint type, const std::string& source)
{
    GLuint shaderObject = 0;

    if (type == GL_VERTEX_SHADER)
    {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    } else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }
    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);
    
    GLint success;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint len = 0;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &len);

        std::string log(len, '\0');

        glGetShaderInfoLog(shaderObject, len, nullptr, log.data());

        std::cerr << log << std::endl;
    }

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexshadersource, 
                        const std::string& fragmentshadersource)
{
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader   = CompileShader(GL_VERTEX_SHADER, vertexshadersource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    glValidateProgram(programObject);

    return programObject;
}

void CreateGraphicsPipeline()
{
    std::cout << "CreateGraphicsPipeline called" << std::endl;
    std::string vertexShaderSource   = utils::LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource = utils::LoadShaderAsString("./shaders/frag.glsl");
    gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void GetOpenGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void InitializeProgram()
{   
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gGraphicsApplicationWindow = SDL_CreateWindow("OpenGL", gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);

    CatchErr(gGraphicsApplicationWindow, "SDL_Window was not able to be created");

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

    CatchErr(gOpenGLContext, "OpenGL context not available"); 

    CatchErr(gladLoadGLLoader(GLADloadproc(SDL_GL_GetProcAddress)), "Failed to init GLAD");

    GetOpenGLVersionInfo();

}

void VertexSpec()
{
    const std::vector<GLfloat> vertexData
    {
        // vertex 0 (pos, color)
        -0.2f, -0.2f, 0.0f,
        0.5f, 0.5f, 1.0f,
        // vertex 1
        0.2f, -0.2f, 0.0f,
        0.5f, 0.5f, 1.0f,
        // vertex 2
        -0.2f, 0.2f, 0.0f,
        0.5f, 0.5f, 1.0f,
        // vertex 3
        0.2f, 0.2f, 0.0f,
        0.5f, 0.5f, 1.0f, 
    };


    // VAO preparation
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);
    // VBO preparation
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertexData.size() * sizeof(GL_FLOAT),
        vertexData.data(),
        GL_STATIC_DRAW
    );
    
    const std::vector<GLuint> indexBufferData {2,0,1, 3,2,1};

    // Setup IBO (EBO)
    glGenBuffers(1, &gIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                indexBufferData.size() * sizeof(GL_FLOAT),
                indexBufferData.data(),
                GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT)*6,
        (void*)0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT)*6,
        (void*)(sizeof(GL_FLOAT)*3)
    );

    glBindVertexArray(0);
    glDisableVertexAttribArray(0); 
    glDisableVertexAttribArray(1);
}

void Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            done = true;

        }
    }

    const bool *state = SDL_GetKeyboardState(NULL);
    const float cameraSpeed = 0.0001f;
    if (state[SDL_SCANCODE_W])
    {
       cameraPos += cameraSpeed * cameraFront;

    }
    if (state[SDL_SCANCODE_S])
    {
       cameraPos -= cameraSpeed * cameraFront;
    }
    if (state[SDL_SCANCODE_A])
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (state[SDL_SCANCODE_D])
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

void PreDraw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);
    GLint mvpLoc = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_MVP");
    glm::mat4 proj  = glm::perspective(glm::radians(45.0f), (float)gScreenWidth/gScreenHeight, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp   = proj * view * model;
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
}

void Draw()
{
    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES,
                    6,
                    GL_UNSIGNED_INT,
                    0
    );
}

void MainLoop()
{
    while (!done)
    {
        Input();

        PreDraw();
    
        Draw();

        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    }
}

void CleanUp()
{
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

int main() 
{
    InitializeProgram();

    CreateGraphicsPipeline();

    VertexSpec();

    MainLoop();

    CleanUp();

    return 0;
}
