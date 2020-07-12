#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  Snake enemy;
  SDL_Point food;
  SDL_Point poison;

  std::random_device dev;
  std::mt19937 engine;
  // random w,h for food
  std::uniform_int_distribution<int> frandom_w;
  std::uniform_int_distribution<int> frandom_h;
  // random w,h for poison
  std::uniform_int_distribution<int> prandom_w;
  std::uniform_int_distribution<int> prandom_h;

  int score{0};

  void PlaceFood();
  void PlacePoison();
  void Update(Snake &obj);
};

#endif