#pragma once

#include <SFML/Graphics.hpp>

namespace structs {
    struct Vec2 {
        int x_;
        int y_;

        Vec2 operator*(const int other) {
            return {x_ * -1, y_ * -1};
        }

        bool operator==(const Vec2& other) {
            return (x_ == other.x_) && (y_ == other.y_);
        }

        Vec2() = default;

        Vec2(int x, int y) :
            x_{x},
            y_{y}
        {};
    };

    struct TextParams {
        sf::Color color;
        int size;
        int xOff;
        int yOff;
        std::string str;
    };
};

namespace templates {
    class Text {
    private:
        sf::RenderWindow *window_;

        sf::Font font_;
        sf::Text textClass_;
    public:
        Text(sf::RenderWindow* windowP, structs::TextParams params) {
            window_ = windowP;

            font_.loadFromFile("font.otf");
            textClass_.setFont(font_);

            setData(params.size, params.color);
            updateText(params.str);
            centre(params.xOff, params.yOff);
        }

        void setData(int size, sf::Color color) {
            textClass_.setCharacterSize(size);
            textClass_.setFillColor(color);
        }

        void updateText(std::string text) {
            textClass_.setString(text);
        }

        void centre(int xOff, int yOff) {
            auto textSize = textClass_.getLocalBounds();
            textClass_.setPosition((504 / 2) - (textSize.width / 2) + xOff, (504 / 2) - textSize.height + yOff);
        }

        void draw() {
            window_->draw(textClass_);
        }
    };
};