name := snake.exe

flags := -lsfml-graphics -lsfml-window -lsfml-system
files := main.cpp snake/snake.cpp NEAT/neat.cpp NEAT/genotype/genotype.cpp NEAT/phenotype/phenotype.cpp

NEATSnake:
	g++ -o $(name) $(files) $(flags)