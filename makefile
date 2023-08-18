name := snake_game

flags := -lsfml-graphics -lsfml-window -lsfml-system --std=c++20
files := main.cpp snake/snake.cpp

NEATSnake:
	g++ -o $(name) $(files) $(flags)
