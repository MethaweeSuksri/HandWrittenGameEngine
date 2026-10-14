#include <stdio.h>

#include <SDL3/SDL.h>
#include <glad/gl.h>

int main(void)
{
    // Initialize SDL's video subsystem.
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Ask SDL for an OpenGL 3.3 Core context.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow(
        "Hello OpenGL",
        800,
        600,
        SDL_WINDOW_OPENGL
    );

    if (window == NULL) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create the OpenGL context.
    SDL_GLContext context = SDL_GL_CreateContext(window);

    if (context == NULL) {
        printf("SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load OpenGL functions using GLAD.
    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);

    if (version == 0) {
        printf("Failed to initialize GLAD\n");
        SDL_GL_DestroyContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    // Main loop.
    int running = 1;

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }

        // Clear the screen.
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Show what we rendered.
        SDL_GL_SwapWindow(window);
    }

    // Cleanup.
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}