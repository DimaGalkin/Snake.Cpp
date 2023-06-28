#include <iostream>

#include <memory>

#include "snake/snake.hpp"
#include "NEAT/neat.hpp"

double sigmoid(double x) { 
        return 1 / (1 + exp(-1 * x));
};

int main() {
    srand(time(NULL));

    Snake::runSnakeGraphics();

    return 0;
}