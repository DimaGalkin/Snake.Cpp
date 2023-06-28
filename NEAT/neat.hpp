#pragma once

#include <iostream>

#include <map>
#include <deque>
#include <memory>
#include <algorithm>

#include <cmath>
#include <time.h> 

#include "../snake/snake.hpp"
#include "genotype/genotype.hpp"
#include "phenotype/phenotype.hpp"
#include "utils.hpp"

namespace NEAT {

class Player {
    private:
        NEAT::Genome* genome;
        int score;

    public:
        Player(InovDescriptor* GIDTptr);

        int action(std::unique_ptr<std::map<int, double>> values);
        void initNetwork(int inputSize, int outputSize);
        void mutate();

        ~Player() {
            delete genome;
        };
};
};