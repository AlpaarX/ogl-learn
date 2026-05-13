#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <iostream>

int gScreenHeight = 840;
int gScreenWidth = 680;
SDL_Window*   gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

template <typename T>
void CatchErr(T instance, std::string message)
{
    if (!instance)
    {
        std::cerr << message << std::endl;
        exit(1);
    }
}

void InitializeProgram()
{
    CatchErr(SDL_Init(SDL_INIT_VIDEO), "Failed to init SDL3");

    gGraphicsApplicationWindow = SDL_CreateWindow("OpenGL", gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);

    CatchErr(gGraphicsApplicationWindow, "SDL_Window was not able to be created");

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

    CatchErr(gOpenGLContext, "OpenGL context not available"); 

    CatchErr(gladLoadGLLoader(GLADloadproc(SDL_GL_GetProcAddress)), "Failed to init GLAD");

}

void MainLoop()
{
    bool done = false;
    while (!done)
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

    SDL_DestroyWindow(gGraphicsApplicationWindow);
}

void CleanUp()
{
    SDL_Quit();
}

int main() {
    InitializeProgram();

    MainLoop();

    CleanUp();
}
