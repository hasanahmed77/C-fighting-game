#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define VELOCITY_CHARACTER 4
#define VELOCITY_BULLET 15

typedef struct
{
    SDL_Rect rect;
    int health;
} Character;

typedef struct
{
    SDL_Rect rect;
    int active;
} Bullet;

void checkBulletCollision(Bullet *bullet, Character *player);

int main(int argc, char *argv[])
{
    // Initializing SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initializing Text Rendering
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf initialization failed. SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    // Loading Fond
    TTF_Font *font = TTF_OpenFont("./src/Fonts/VT323-Regular.ttf", 24);
    if (!font)
    {
        printf("Font loading failed. SDL_ttf Error: %s\n", TTF_GetError());
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

    // Loading stormtrooper's bullet
    SDL_Texture *stormtrooperBulletTexture = IMG_LoadTexture(renderer, "./src/Assets/stormtrooper-blaster-2.png");
    if (!stormtrooperBulletTexture)
    {
        printf("Unable to create stormtrooper BULLET. SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyTexture(stormtrooperTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Bullet bullets[10];
    int bulletIndex = 0;

    for (int i = 0; i < 10; i++)
    {
        bullets[i].active = 0;
        bullets[i].rect.x = -100;
        bullets[i].rect.y = -100;
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
    Character stormtrooper = {{100, 400, 40, 50}, 10};
    Character bobaFett = {{800, 400, 30, 50}, 10};

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
        if (keyState[SDL_SCANCODE_W] && stormtrooper.rect.y > 0)
        {
            stormtrooper.rect.y -= VELOCITY_CHARACTER;
        }
        // DOWN boundary
        if (keyState[SDL_SCANCODE_S] && stormtrooper.rect.y < WINDOW_HEIGHT - stormtrooper.rect.h)
        {
            stormtrooper.rect.y += VELOCITY_CHARACTER;
        }
        // LEFT Boundary
        if (keyState[SDL_SCANCODE_A] && stormtrooper.rect.x > 0)
        {
            stormtrooper.rect.x -= VELOCITY_CHARACTER;
        }
        // RIGHT Boundary
        if (keyState[SDL_SCANCODE_D] && stormtrooper.rect.x < WINDOW_WIDTH / 2 - stormtrooper.rect.w)
        {
            stormtrooper.rect.x += VELOCITY_CHARACTER;
        }

        // Boba Fett
        // UP boundary
        if (keyState[SDL_SCANCODE_UP] && bobaFett.rect.y > 0)
        {
            bobaFett.rect.y -= VELOCITY_CHARACTER;
        }
        // DOWN boundary
        if (keyState[SDL_SCANCODE_DOWN] && bobaFett.rect.y < WINDOW_HEIGHT - bobaFett.rect.h)
        {
            bobaFett.rect.y += VELOCITY_CHARACTER;
        }
        // LEFT Boundary
        if (keyState[SDL_SCANCODE_LEFT] && bobaFett.rect.x > WINDOW_WIDTH / 2 + bobaFett.rect.w)
        {
            bobaFett.rect.x -= VELOCITY_CHARACTER;
        }
        // RIGHT Boundary
        if (keyState[SDL_SCANCODE_RIGHT] && bobaFett.rect.x < WINDOW_WIDTH - bobaFett.rect.w)
        {
            bobaFett.rect.x += VELOCITY_CHARACTER;
        }

        // Firing stormtrooper bullets
        if (keyState[SDL_SCANCODE_SPACE])
        {
            for (int i = 0; i < 10; i++)
            {
                if (!bullets[bulletIndex].active)
                {
                    bullets[i].rect.x = stormtrooper.rect.x + stormtrooper.rect.w;
                    bullets[i].rect.y = stormtrooper.rect.y + stormtrooper.rect.h / 2 - 5;
                    bullets[i].rect.w = 40;
                    bullets[i].rect.h = 10;
                    bullets[i].active = 1;
                }
            }
        }

        // Moving the active bullets
        for (int i = 0; i < 10; i++)
        {
            if (bullets[i].active)
            {
                bullets[i].rect.x += VELOCITY_BULLET;

                checkBulletCollision(&bullets[i], &bobaFett);

                if (bullets[i].rect.x > WINDOW_WIDTH)
                    bullets[i].active = 0;
            }
        }

        // Rendering a black screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Rendering the background image
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        SDL_RenderCopy(renderer, stormtrooperTexture, NULL, &stormtrooper.rect);
        SDL_RenderCopy(renderer, bobaFettTexture, NULL, &bobaFett.rect);

        for (int i = 0; i < 10; i++)
        {
            if (bullets[i].active)
            {
                SDL_RenderCopy(renderer, stormtrooperBulletTexture, NULL, &bullets[i].rect);
            }
        }

        // Displaying Health
        char healthText[50];
        sprintf(healthText, "Health: %d", bobaFett.health);

        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, healthText, white);
        if (!textSurface)
        {
            printf("Text surface creation failed! SDL_ttf Error: %s\n", TTF_GetError());
        }

        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        if (!textTexture)
        {
            printf("Text texture creation failed! SDL_ttf Error: %s\n", TTF_GetError());
        }

        SDL_Rect textRect = {10, 10, 0, 0};

        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

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

void checkBulletCollision(Bullet *bullet, Character *player)
{
    if (bullet->active && SDL_HasIntersection(&bullet->rect, &player->rect))
    {
        player->health -= 1;

        bullet->active = 0;
    }
}
