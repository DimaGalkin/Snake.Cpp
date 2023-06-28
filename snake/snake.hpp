#pragma once

#include <iostream>

// data structures
#include <deque>
#include <unordered_map>
#include <unordered_set>

// utils
#include <cstdlib>
#include <chrono>

// graphics
#include <SFML/Graphics.hpp>

#include "../NEAT/phenotype/phenotype.hpp"
#include "../NEAT/genotype/genotype.hpp"
#include "../NEAT/neat.hpp"
#include "utils/utils.hpp"

namespace Utils {
    class FrameTimeHandler {
        private:
            std::chrono::_V2::system_clock::time_point start;
            std::chrono::_V2::system_clock::time_point now;
            std::chrono::microseconds duration;
        public:
            void setStart();
            double diff();
    };
};

namespace Snake {
    class SnakeGame {
    private:
        sf::RenderWindow* window;
        sf::RectangleShape apple;
        std::deque<sf::RectangleShape> snake;
        std::deque<structs::Vec2> snakePos;
        std::unordered_set<int> blackSquares;
        structs::Vec2 movVec;
        structs::Vec2 direction = {1, 0};
        structs::Vec2 applePos = randomVec2();
        int applesEaten = 0;
        int snakeLength = 0;
        int borderSize = 2;
        sf::Color bord = sf::Color{124, 108, 230};
        std::vector<sf::RectangleShape> borders;
        bool graphics = true;
        bool gameOver = false;
    public:
        int scaleFactor = 10;

        SnakeGame(sf::RenderWindow *window);

        int Vec2Int(int x, int y);
        double Vec2Double(int x, int y);

        sf::RectangleShape createSquare(sf::Color color);

        void createSprites(int x, int y, int length);

        void drawSprites();

        void checkBounds();

        void checkCollision();

        void touchingApple();

        void moveSnake();

        void gameLoop();

        void initialRender();

        void handleKey(structs::Vec2 vec);

        structs::Vec2 randomVec2();

        void drawBorder();

        int getApplesEaten();

        void AIKey(int key);

        bool gameStatus();

        std::unique_ptr<std::map<int, double>> getBoard();
    };

    void checkKey(SnakeGame* snake);
    void runSnakeGraphics();
    void runSnakeHeadless(int idef);
};