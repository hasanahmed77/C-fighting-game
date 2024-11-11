#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define VELOCITY 4

int main(int argc, char *argv[])
{
    // Initializing SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Creating a window
    SDL_Window *window = SDL_CreateWindow("STAR WARS",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Creating a renderer
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, render_flags);
    if (!renderer)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Creating surfaces for pictures
    SDL_Surface *backgroundSurface = IMG_Load("./src/Assets/background.jpg");
    if (!backgroundSurface)
    {
        printf("Unable to load background image. SDL_image Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Converting the surface to texture
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    if (!backgroundTexture)
    {
        printf("Unable to create texture from surface. SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Loading stormtrooper image as texture
    SDL_Texture *stormtrooperTexture = IMG_LoadTexture(renderer, "./src/Assets/stormtrooper-2.png");
    if (!stormtrooperTexture)
    {
        printf("Unable to create stormtrooper picture. SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Loading boba fett image as texture
    SDL_Texture *bobaFettTexture = IMG_LoadTexture(renderer, "./src/Assets/boba-fett-2.png");
    if (!bobaFettTexture)
    {
        printf("Unable to create boba fett picture. SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initializing characters
    SDL_Rect stormtrooperRect = {100, 400, 40, 50};
    SDL_Rect bobaFettRect = {800, 400, 30, 50};

    // Main loop
    int running = 1;
    SDL_Event e;
    while (running)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        const Uint8 *keyState = SDL_GetKeyboardState(NULL);

        // INPUT & BOUNDARIES
        // Stormtrooper
        // UP boundary
        if (keyState[SDL_SCANCODE_W] && stormtrooperRect.y > 0)
        {
            stormtrooperRect.y -= VELOCITY;
        }
        // DOWN boundary
        if (keyState[SDL_SCANCODE_S] && stormtrooperRect.y < WINDOW_HEIGHT - stormtrooperRect.h)
        {
            stormtrooperRect.y += VELOCITY;
        }
        // LEFT Boundary
        if (keyState[SDL_SCANCODE_A] && stormtrooperRect.x > 0)
        {
            stormtrooperRect.x -= VELOCITY;
        }
        // RIGHT Boundary
        if (keyState[SDL_SCANCODE_D] && stormtrooperRect.x < WINDOW_WIDTH / 2 - stormtrooperRect.w)
        {
            stormtrooperRect.x += VELOCITY;
        }

        // Boba Fett
        // UP boundary
        if (keyState[SDL_SCANCODE_UP] && bobaFettRect.y > 0)
        {
            bobaFettRect.y -= VELOCITY;
        }
        // DOWN boundary
        if (keyState[SDL_SCANCODE_DOWN] && bobaFettRect.y < WINDOW_HEIGHT - bobaFettRect.h)
        {
            bobaFettRect.y += VELOCITY;
        }
        // LEFT Boundary
        if (keyState[SDL_SCANCODE_LEFT] && bobaFettRect.x > WINDOW_WIDTH / 2 + bobaFettRect.w)
        {
            bobaFettRect.x -= VELOCITY;
        }
        // RIGHT Boundary
        if (keyState[SDL_SCANCODE_RIGHT] && bobaFettRect.x < WINDOW_WIDTH - bobaFettRect.w)
        {
            bobaFettRect.x += VELOCITY;
        }

        // Rendering a black screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Rendering the background image
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        SDL_RenderCopy(renderer, stormtrooperTexture, NULL, &stormtrooperRect);
        SDL_RenderCopy(renderer, bobaFettTexture, NULL, &bobaFettRect);

        // Updating the screen
        SDL_RenderPresent(renderer);
    }

    // Cleaning up and exiting
    SDL_DestroyTexture(stormtrooperTexture);
    SDL_DestroyTexture(bobaFettTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
