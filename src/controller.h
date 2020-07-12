#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <random>
#include <chrono>
#include "snake.h"

class Controller {
 public:
  Controller();
  void HandleInput(bool &running, Snake &snake);
  void RandomInput(Snake &enemy);

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
  // random value for non-main snake direction
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random;
  std::uniform_int_distribution<int> rtime;
  int t_update;
  std::chrono::system_clock::time_point t_start;
};

#endif