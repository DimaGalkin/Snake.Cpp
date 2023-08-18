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
        sf::RenderWindow* window_;
        sf::RectangleShape apple_;
        std::deque<sf::RectangleShape> snake_;
        std::deque<structs::Vec2> snakePos_;
        std::unordered_set<int> blackSquares_;
        structs::Vec2 movVec_;
        structs::Vec2 direction_ = {1, 0};
        structs::Vec2 applePos_ = randomVec2();
        int applesEaten_ = 0;
        int snakeLength_ = 0;
        int borderSize_ = 2;
        sf::Color bord_ = sf::Color{124, 108, 230};
        std::vector<sf::RectangleShape> borders_;
        bool graphics_ = true;
        bool gameOver_ = false;
    public:
        int scaleFactor_ = 10;

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

        bool gameStatus();
    };

    void checkKey(SnakeGame* snake);
    void runGraphics();
};
