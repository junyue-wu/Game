#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

Controller::Controller()
          : engine(dev()),
            random(0, 3),
            rtime(1000, 3000),
            t_update(rtime(engine)),
            t_start(std::chrono::system_clock::now()) {}

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    }
    else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
      }
    }
  }
}

void Controller::RandomInput(Snake &enemy) {
  int duration = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now() - t_start).count();
  if (duration >= t_update) {
    int ran_direction = random(engine);
    switch(ran_direction) {
      case 0:
        ChangeDirection(enemy, Snake::Direction::kUp,
                        Snake::Direction::kDown);
        break;
      case 1:
        ChangeDirection(enemy, Snake::Direction::kDown,
                        Snake::Direction::kUp);
        break;
      case 2:
        ChangeDirection(enemy, Snake::Direction::kLeft,
                        Snake::Direction::kRight);
        break;
      case 3:
        ChangeDirection(enemy, Snake::Direction::kRight,
                        Snake::Direction::kLeft);
        break;
    }
    
    t_start = std::chrono::system_clock::now();
    t_update = rtime(engine);
  }
}
