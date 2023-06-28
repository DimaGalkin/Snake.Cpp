#include "neat.hpp"

NEAT::Player::Player(InovDescriptor* GIDTptr) {
    genome = new Genome(GIDTptr);

    //snake = game;
}

int NEAT::Player::action(std::unique_ptr<std::map<int, double>> values) {
    int direction = genome->runGenome(std::move(values));

    return direction;
}

void NEAT::Player::mutate() {
    genome->mutate();
    genome->generatePhenotype();
}

void NEAT::Player::initNetwork(int inputSize, int outputSize) {
    for (int id = 0; id < inputSize; ++id) {
        genome->addNode(genome->maxNode, SENSOR);
    }

    for (int id = 0; id < outputSize; ++id) {
        genome->addNode(genome->maxNode, OUTPUT);
    }

    int numConnections = genome->minMaxRandom(inputSize, ((inputSize * outputSize) / 2) + 1);

    for (int i = 0; i < numConnections; ++i) {
        genome->addConnection(genome->minMaxRandom(0, inputSize), genome->minMaxRandom(inputSize, inputSize + outputSize), genome->minMaxFLPR(-2, 2), true);
    }

    genome->generatePhenotype();
}