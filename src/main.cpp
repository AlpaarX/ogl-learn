#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>

int gScreenHeight = 840;
int gScreenWidth = 680;
SDL_Window*   gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

// Stop MainLoop variable
bool done = false;

// VAO
GLuint gVertexArrayObject = 0;

// VBO
GLuint gVertexBufferObject = 0;

// Program Object for shaders
GLuint gGraphicsPipelineShaderProgram = 0;

const std::string gVertexShaderSource = 
"#version 430 core\n"
"in vec4 position;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
"}\n";

const std::string gFragmentShaderSource = 
"#version 430 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
"}\n";

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
    GLuint shaderObject;

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
    gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 6);
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
    const std::vector<GLfloat> vertexPos
    {   //x     y      z
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f
    };

    // VAO preparation
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);
   // VBO preparation
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertexPos.size() * sizeof(GLfloat),
        vertexPos.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glBindVertexArray(0);
    glEnableVertexAttribArray(0);
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
}

void PreDraw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(0.204f, 0.698f, 0.878f, 1.f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw()
{
    
    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

    glDrawArrays(GL_TRIANGLES, 0, 3);

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

    VertexSpec();

    CreateGraphicsPipeline();

    MainLoop();

    CleanUp();

    return 0;
}
