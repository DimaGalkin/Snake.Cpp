name := snake_game

flags := -lsfml-graphics -lsfml-window -lsfml-system --std=c++20 -fsanitize=address -static-libasan -g3
files := main.cpp snake/snake.cpp

Snake:
	g++ -o $(name) $(files) $(flags)
