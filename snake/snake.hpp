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
            std::chrono::_V2::system_clock::time_point start_;
            std::chrono::_V2::system_clock::time_point now_;
            std::chrono::microseconds duration_;
        public:
            void setStart();
            double diff();
    };
};

namespace Snake {
    class SnakeGame {
    private:
        sf::RenderWindow* window_;

        const sf::Color bord_ = sf::Color{124, 108, 230};

        sf::RectangleShape apple_;
        
        std::vector<sf::RectangleShape> borders_;

        std::deque<sf::RectangleShape> snake_;
        std::deque<structs::Vec2> snakePos_;
        
        std::unordered_set<int> blackSquares_;
        
        structs::Vec2 movVec_;
        structs::Vec2 direction_;
        structs::Vec2 applePos_;
        
        int applesEaten_ = 0;
        int snakeLength_ = 0;
        const int borderSize_ = 2;
        const int scaleFactor_ = 10;

        const bool graphics_ = true;
        bool gameOver_ = false;

        structs::Vec2 randomVec2();

        sf::RectangleShape createSquare(sf::Color color);

        void drawSprites();
        void checkBounds();
        void checkCollision();
        void touchingApple();
        void moveSnake();

        int Vec2Int(int x, int y);

        public:
            SnakeGame(sf::RenderWindow *window);

            int getScaleFactor();

            void createSprites(int x, int y, int length);
            void initialRender();
            void gameLoop();
            void handleKey(structs::Vec2 vec);
            void drawBorder();

            bool gameStatus();

            int getApplesEaten();
    };

    void checkKey(SnakeGame* snake);
    void runGraphics();
};
