#include "phenotype.hpp"

void NEAT::Phenotype::print() {
    std::cout << "\nSensor Layer [" << "\n";
    for ( const auto &[id, neuron] : SensorLayer ) {
        std::cout << "  Neuron: " << id << "\n";
        std::cout << "  Connected to: ";
        for (int i = 0; i < neuron.connectedTo.size(); ++i) {
            std::cout << neuron.connectedTo.at(i) << " ";
        }
        std::cout << "\n  Weights: ";
        for (int i = 0; i < neuron.weights.size(); ++i) {
            std::cout << neuron.weights.at(i).weight << " ";
        }
        std::cout << "\n\n";
    }
    std::cout << "]\n";

    std::cout << "\nHidden Layer [" << "\n";
    for ( const auto &[id, neuron] : HiddenLayer ) {
        std::cout << "  Neuron: " << id << "\n";
        std::cout << "  Connected to: ";
        for (int i = 0; i < neuron.connectedTo.size(); ++i) {
            std::cout << neuron.connectedTo.at(i) << " ";
        }
        std::cout << "\n  Weights: ";
        for (int i = 0; i < neuron.weights.size(); ++i) {
            std::cout << neuron.weights.at(i).weight << " ";
        }
        std::cout << "\n\n";
    }
    std::cout << "]\n\n";
}

void NEAT::Phenotype::generateNetwork(std::deque<ConnectionGene>* connections, std::deque<NodeGene>* nodes) {
    for (int i = 0; i < nodes->size(); ++i) {
        switch (nodes->at(i).type) {
            case SENSOR:
                SensorLayer[nodes->at(i).id] = {SENSOR, {}, {}, 0};
                break;
            case HIDDEN:
                HiddenLayer[nodes->at(i).id] = {HIDDEN, {}, {}, 0};
                break;
            case OUTPUT:
                OutputLayer[nodes->at(i).id] = {OUTPUT, {}, {}, 0};
                break;
        }
    }

    for (int i = 0; i < connections->size(); ++i) {
            if (SensorLayer.count(connections->at(i).in) > 0) {

                SensorLayer[connections->at(i).in].connectedTo.push_back(connections->at(i).out);
                SensorLayer[connections->at(i).in].weights.push_back(NEAT::Weight{connections->at(i).weight, connections->at(i).enabled});

            } else if (HiddenLayer.count(connections->at(i).in) > 0) {

                HiddenLayer[connections->at(i).in].connectedTo.push_back(connections->at(i).out);
                HiddenLayer[connections->at(i).in].weights.push_back(NEAT::Weight{connections->at(i).weight, connections->at(i).enabled});
            }
    }
}

int NEAT::Phenotype::execNetwork(std::unique_ptr<std::map<int, double>> values) {
    NEAT::Phenotype::nullifyNetwork();

    for ( const auto &p : *values ) {
        SensorLayer[p.first].value = p.second;
    }

    for ( auto &[id, neuron] : SensorLayer ) {
        for (int i = 0; i < neuron.weights.size(); ++i) {
            int connectedToPos = neuron.connectedTo.at(i);
            NEAT::Weight weightPos = neuron.weights.at(i);

            if (HiddenLayer.count(connectedToPos) > 0 && weightPos.enabled) {
                HiddenLayer[connectedToPos].value += weightPos.weight * sigmoid(neuron.value);
            } else if (OutputLayer.count(connectedToPos) > 0 && weightPos.enabled) {
                OutputLayer[connectedToPos].value += weightPos.weight * sigmoid(neuron.value);
            }
        }
    }

    for ( auto &[id, neuron] : HiddenLayer ) {
        for (int i = 0; i < neuron.weights.size(); ++i) {
            int connectedToPos = neuron.connectedTo.at(i);
            NEAT::Weight weightPos = neuron.weights.at(i);

            if (HiddenLayer.count(connectedToPos) > 0 && weightPos.enabled) {
                HiddenLayer[connectedToPos].value += weightPos.weight * sigmoid(neuron.value);
            } else if (OutputLayer.count(connectedToPos) > 0 && weightPos.enabled) {
                OutputLayer[connectedToPos].value += weightPos.weight * sigmoid(neuron.value);
            }
        }
    }

    double max = 0;
    int maxIdef = 0;

    bool first = true;
    
    for ( const auto &[id, neuron] : OutputLayer ) {
        if (sigmoid(neuron.value) > max || first) {
            maxIdef = id;
            max = sigmoid(neuron.value);
        }

        first = false;

        std::cout << "Output: " << id << " Value: " << sigmoid(neuron.value) << "\n";
    }

    return maxIdef;
}

double NEAT::Phenotype::sigmoid(double x) {
    return 1 / (1 + std::exp(-x));
}

void NEAT::Phenotype::nullifyNetwork() {
    for ( const auto &p : SensorLayer ) {
        SensorLayer[p.first].value = 0;
    }

    for ( const auto &p : HiddenLayer ) {
        HiddenLayer[p.first].value = 0;
    }

    for ( const auto &p : OutputLayer ) {
        OutputLayer[p.first].value = 0;
    }
}