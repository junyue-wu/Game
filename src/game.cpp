#include "game.h"
#include <iostream>
//#include <thread>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height, 1, true),
      enemy(grid_width, grid_height, 1, false),
      engine(dev()),
      frandom_w(0, static_cast<int>(grid_width-1)), // -1 to make sure inside the window
      frandom_h(0, static_cast<int>(grid_height-1)),
      prandom_w(0, static_cast<int>(grid_width-1)),
      prandom_h(0, static_cast<int>(grid_height-1)) {
  // initialize the size of enemy = 3
  enemy.GrowBody();
  Update(enemy, enemy);
  enemy.GrowBody();
  Update(enemy, enemy);
  
  PlaceFood();
  PlacePoison();
}

void Game::Run(Controller &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.RandomInput(enemy);
    Update(enemy, enemy);

    controller.HandleInput(running, snake);
    Update(snake, enemy);

    renderer.Render(snake, enemy, food, poison);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = frandom_w(engine);
    y = frandom_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlacePoison() {
  int x, y;
  while (true) {
    x = prandom_w(engine);
    y = prandom_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if ((!snake.SnakeCell(x, y)) && x != food.x && y != food.y) {
      poison.x = x;
      poison.y = y;
      return;
    }
  }
}

void Game::Update(Snake &obj, Snake &enemy) {
  if (!obj.alive) return;

  obj.Update(enemy);

  int new_x = static_cast<int>(obj.head_x);
  int new_y = static_cast<int>(obj.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    // enemy snake does not get score
    if (obj.isMainsnake == true) {
      score++;
    }
    PlaceFood();
    // Grow snake and increase speed.
    obj.GrowBody();
    obj.speed += 0.02;
  }

  // check if there's poison over there
  if (poison.x == new_x && poison.y == new_y) {
    // enemy snake does not get score
    if (obj.isMainsnake == true) {
      score = score - 2;
    }
    PlacePoison();
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }