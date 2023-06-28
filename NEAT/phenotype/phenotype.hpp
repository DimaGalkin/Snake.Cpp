#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <cmath>

#include "../utils.hpp"

namespace NEAT {
class Phenotype {
    public:
        std::map<int, Neuron> SensorLayer;
        std::map<int, Neuron> HiddenLayer;
        std::map<int, Neuron> OutputLayer;

    void print();

    void generateNetwork(std::deque<ConnectionGene>* connections, std::deque<NodeGene>* nodes);

    int execNetwork(std::unique_ptr<std::map<int, double>> values);

    double sigmoid(double x);

    void nullifyNetwork();
};
};