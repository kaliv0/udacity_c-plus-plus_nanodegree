#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
        : screen_width(screen_width),
          screen_height(screen_height),
          grid_width(grid_width),
          grid_height(grid_height) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create Window
    sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, screen_width,
                                  screen_height, SDL_WINDOW_SHOWN);

    if (nullptr == sdl_window) {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create renderer
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == sdl_renderer) {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
}

Renderer::~Renderer() {
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void Renderer::Render(Snake const *snake1, Snake const *snake2, Food &food) {
    SDL_Rect block;
    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;

    // Clear screen
    SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer);


    // Render food
    switch (food.GetFoodType()) {
        case FoodType::GOOD:
            SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x99, 0x33, 0xFF);
            break;
        case FoodType::BAD:
            SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
            break;
        case FoodType::POWER:
            SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0xFF, 0xFF);
            break;
        case FoodType::SLOW:
            SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0xFF, 0xFF);
            break;
        default:
            break;
    }

    block.x = food.GetFoodLocation().x * block.w;
    block.y = food.GetFoodLocation().y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);

    // Render snake1's body
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (SDL_Point const &point: snake1->body) {
        block.x = point.x * block.w;
        block.y = point.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

    // Render snake1's head
    block.x = static_cast<int>(snake1->head_x) * block.w;
    block.y = static_cast<int>(snake1->head_y) * block.h;
    if (snake1->alive) {
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
    } else {
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    }
    SDL_RenderFillRect(sdl_renderer, &block);

    if (snake2 != nullptr) {
        // Render snake2's body
        SDL_SetRenderDrawColor(sdl_renderer, 0xCC, 0xFF, 0xFF, 0xFF);
        for (SDL_Point const &point: snake2->body) {
            block.x = point.x * block.w;
            block.y = point.y * block.h;
            SDL_RenderFillRect(sdl_renderer, &block);
        }

        // Render snake2's head
        block.x = static_cast<int>(snake2->head_x) * block.w;
        block.y = static_cast<int>(snake2->head_y) * block.h;
        if (snake2->alive) {
            SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x99, 0x00, 0xFF);
        } else {
            SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
        }
        SDL_RenderFillRect(sdl_renderer, &block);
    }


    // Update Screen
    SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
    std::string title{"Player Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
    SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::UpdateWindowTitle(int score1, int score2, int fps) {
    std::string title{
            "Player_1 Score: " + std::to_string(score1) + " Player_2 Score: " + std::to_string(score2) + " FPS: " +
            std::to_string(fps)};
    SDL_SetWindowTitle(sdl_window, title.c_str());
}