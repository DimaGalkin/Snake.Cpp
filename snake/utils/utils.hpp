#pragma once

#include <SFML/Graphics.hpp>

namespace structs {
    struct Vec2 {
        int x;
        int y;

        Vec2 operator*(const int other) {
            return {x * -1, y * -1};
        }

        bool operator==(const Vec2 other) {
            return (x == other.x) && (y == other.y);
        }
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
        sf::RenderWindow *window;

        sf::Font font;
        sf::Text textClass;
    public:
        Text(sf::RenderWindow *windowP, structs::TextParams params) {
            window = windowP;

            font.loadFromFile("font.otf");
            textClass.setFont(font);

            setData(params.size, params.color);
            updateText(params.str);
            centre(params.xOff, params.yOff);
        }

        void setData(int size, sf::Color color) {
            textClass.setCharacterSize(size);
            textClass.setFillColor(color);
        }

        void updateText(std::string text) {
            textClass.setString(text);
        }

        void centre(int xOff, int yOff) {
            auto textSize = textClass.getLocalBounds();
            textClass.setPosition((504 / 2) - (textSize.width / 2) + xOff, (504 / 2) - textSize.height + yOff);
        }

        void draw() {
            window->draw(textClass);
        }
    };
};