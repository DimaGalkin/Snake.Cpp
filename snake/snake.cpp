#include "snake.hpp"

void Utils::FrameTimeHandler::setStart() {
    start = std::chrono::high_resolution_clock::now();
}

double Utils::FrameTimeHandler::diff() {
    now = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);

    return duration.count() / 1000;
}

Snake::SnakeGame::SnakeGame(sf::RenderWindow *window) {
    window_ = window;

    // create borders
    sf::RectangleShape top (sf::Vector2f(54 * scaleFactor_, 2));
    top.setFillColor(bord_);
    top.setPosition(0, 0);
    borders_.push_back(top);

    sf::RectangleShape bottom(sf::Vector2f(54 * scaleFactor_, 2));
    bottom.setFillColor(bord_);
    bottom.setPosition(0, 52 * scaleFactor_);
    borders_.push_back(bottom);

    sf::RectangleShape left(sf::Vector2f(2, 54 * scaleFactor_));
    left.setFillColor(bord_);
    left.setPosition(0, 0);
    borders_.push_back(left);

    sf::RectangleShape right(sf::Vector2f(2, 54 * scaleFactor_));
    right.setFillColor(bord_);
    right.setPosition(52 * scaleFactor_, 0);
    borders_.push_back(right);


    // seed random number generator
    srand(time(0));

    // set starting direction_
    movVec_ = {1, 0};
}

// creates unique integer from a given vector (used for unordered_set)
int Snake::SnakeGame::Vec2Int(int x, int y) {
    return (x << 16)  + y;
}

// creates a square with a fixed size (1 block = 10px)
sf::RectangleShape Snake::SnakeGame::createSquare(sf::Color color) {
    sf::RectangleShape square(sf::Vector2f(scaleFactor_, scaleFactor_));
    square.setFillColor(color);
    return square;
}

// creates the snake and the apple
void Snake::SnakeGame::createSprites(int x, int y, int length) {
    snakeLength_ = length;

    for (int i = 0; i < length; ++i) {
        snakePos_.push_back(structs::Vec2{x + (i * scaleFactor_), y});
        blackSquares_.insert(Vec2Int(x + (i * scaleFactor_), y));

        auto square = createSquare(sf::Color::Black);

        square.setPosition(snakePos_.at(i).x + borderSize_, snakePos_.at(i).y + borderSize_);
        snake_.push_back(square);
    }

    apple_ = createSquare(sf::Color::Red);
    apple_.setPosition(applePos_.x + borderSize_, applePos_.y + borderSize_);
}

// draws the boxes held in the snake vector and the apple
void Snake::SnakeGame::drawSprites() {
    if (graphics_) {
        window_->draw(apple_);
    }

    for (const auto& square : snake_) {
        if (graphics_) {
            window_->draw(square);
        }
    }
}

// checks if the snake is out of bounds (touching the borders)
void Snake::SnakeGame::checkBounds() {
    structs::Vec2 head = snakePos_.at(snakePos_.size() - 1);

    gameOver_ = head.x < 0 || head.x > (50 * scaleFactor_) - borderSize_ || head.y < 0 || head.y > (50 * scaleFactor_) - borderSize_;
}

/*
Every time the snake moves, all of its positions are stored in a set so if the snake has a length of 5, 
there should be 5 positions in the set, if not then two squares are overlapping as same square cannot
be stored twice in a set. This speeds up calculations as this is O(1) instead of O(n) if we were to check
if the snake is overlapping by iterating through the snakePos_ vector.
*/
void Snake::SnakeGame::checkCollision() {
    if (blackSquares_.size() != snakeLength_) {
        if (graphics_ == true) {
            window_->clear();
        }

        gameOver_ = true;
    }
}

/*
If the snake is touching the apple, a new square is added to the snake at the apples location
and a new apple is generated.
*/
void Snake::SnakeGame::touchingApple() {
    if (snakePos_.at(snakePos_.size() - 1) == applePos_) {
        int snakeSizem1 = snakePos_.size() - 1;

        //xy position of square in front of snake
        int newx = snakePos_.at(snakeSizem1).x + movVec_.x * scaleFactor_;
        int newy = snakePos_.at(snakeSizem1).y + movVec_.y * scaleFactor_;
        
        // add these new coordinates to the snake positions & Black squares
        snakePos_.push_back(structs::Vec2{newx, newy});
        blackSquares_.insert(Vec2Int(newx, newy));

        // add new square to snake and set its position
        auto square = createSquare(sf::Color::Black);
        square.setPosition(newx + borderSize_, newy + borderSize_);
        snake_.push_back(square);


        // generate new apple
        applePos_ = randomVec2();
        apple_.setPosition(applePos_.x + borderSize_, applePos_.y + borderSize_);

        snakeLength_ += 1;
        applesEaten_ += 1;
    }
}

/*
The trailing square of the snake is moved to the position in front of the snake, this is once again an optimization
as when snake gets larger we will still are only moving one square instead of the whole snake.
*/
void Snake::SnakeGame::moveSnake() {
    int snakeSizem1 = snakePos_.size() - 1;

    // position of square in front of snake
    int newx = snakePos_.at(snakeSizem1).x + movVec_.x * scaleFactor_;
    int newy = snakePos_.at(snakeSizem1).y + movVec_.y * scaleFactor_;

    // add new position to snake positions
    snakePos_.push_back(structs::Vec2{newx, newy});
    blackSquares_.insert(Vec2Int(newx, newy));

    // remove the trailing square
    structs::Vec2 first = snakePos_.at(0);

    blackSquares_.erase(Vec2Int(first.x, first.y));
    snakePos_.erase(snakePos_.begin());

    // move the last square to the front
    auto it = snake_.begin();
    std::rotate(it, it + 1, snake_.end());

    // draw the square which was at the back at the front
    snake_.at(snakeSizem1).setPosition(snakePos_.at(snakeSizem1).x + borderSize_, snakePos_.at(snakeSizem1).y + borderSize_);

    direction_ = movVec_;
}

// called every cycle
void Snake::SnakeGame::gameLoop() {
    if (graphics_) {
        window_->clear(sf::Color::White);
    }
    
    drawBorder();
    touchingApple();
    moveSnake();
    checkBounds();
    drawSprites();
    checkCollision();

    if (graphics_) {
        window_->display();
    }
}

void Snake::SnakeGame::initialRender() {
    if (graphics_) {
        window_->clear(sf::Color::White);
    }
    
    drawBorder();
    drawSprites();
    
    if (graphics_) {
        window_->display();
    }
}

// checks that the player is not trying to move into the snake
void Snake::SnakeGame::handleKey(structs::Vec2 vec) {
    if (vec != direction_ * -1) {
        movVec_ = vec;
    }
}

// random position in px not units
structs::Vec2 Snake::SnakeGame::randomVec2() {
    int x = rand() % 491; // between 0 and 490 inclusive
    int y = rand() % 491;

    x = ((x + 5) / 10) * scaleFactor_; // round to nearest 10
    y = ((y + 5) / 10) * scaleFactor_;

    int intVec = Vec2Int(x, y);

    while(blackSquares_.find(intVec) != blackSquares_.end()) {
        x += scaleFactor_;
        
        if (x > 49 * scaleFactor_) {
            x = 0;
            y += scaleFactor_;
        }

        if (y > 49 * scaleFactor_) {
            y = 0;
        }
    }

    return structs::Vec2{x, y};
}

// combines predefied rectangles to create a border
void Snake::SnakeGame::drawBorder() {
    if (graphics_) {
        window_->draw(borders_[0]);
        window_->draw(borders_[1]);
        window_->draw(borders_[2]);
        window_->draw(borders_[3]);
    }
}

int Snake::SnakeGame::getApplesEaten() {
    return applesEaten_;
}

bool Snake::SnakeGame::gameStatus() {
    return gameOver_;
}

// changes the direction_ vector of the snake based on the key pressed
void Snake::checkKey(SnakeGame* snake) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        snake->handleKey({-1, 0});
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        snake->handleKey({1, 0});
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        snake->handleKey({0, -1});
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        snake->handleKey({0, 1});
    }
}

void Snake::runGraphics() {
    sf::RenderWindow window(
        sf::VideoMode(504, 504),
        "Snake v1.0.0b"
    );

    Snake::SnakeGame snake (&window);
    Utils::FrameTimeHandler frameTime;

    snake.createSprites(25 * snake.scaleFactor_, 25 * snake.scaleFactor_, 3);
    snake.initialRender();

    frameTime.setStart();

    while (window.isOpen() && snake.gameStatus() == false) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // 200ms per cycle
        if (frameTime.diff() >= 200) {
            frameTime.setStart();

            Snake::checkKey(&snake);

            snake.gameLoop();
        }

        // handle key input every cylce not just every 200ms
        Snake::checkKey(&snake);
    }

    templates::Text title = templates::Text(&window, structs::TextParams
    {   
        sf::Color::Black,
        32,
        0,
        -32,
        "Apples Eaten:"
    });

    templates::Text appleText = templates::Text(&window, structs::TextParams
    {   
        sf::Color::Red,
        32,
        0,
        14,
        std::to_string(snake.getApplesEaten())
    });

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color::White);
        
        title.draw();
        appleText.draw();
        snake.drawBorder();
        
        window.display();
    }
}
