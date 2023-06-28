#pragma once

#include <deque>

namespace NEAT {
    
enum GeneType {
    SENSOR,
    HIDDEN,
    OUTPUT
};

struct ConnectionGene {
    int in;
    int out;
    double weight;
    int innovation;
    bool enabled;

    bool operator==(const ConnectionGene other) {
        return (in == other.in) && (out == other.out);
    }
};

struct NodeGene {
    int id;
    GeneType type;
};

struct inovDescriptor {
    int in;
    int out;
    int innovation;

    bool operator>>(const inovDescriptor other) {
        return (in == other.in) && (out == other.out);
    }
};

struct InovExists {
    int innovation;
    bool exists;
};

struct Weight {
    double weight;
    bool enabled;
};

struct Neuron {
    GeneType type;

    std::deque<int> connectedTo;
    std::deque<Weight> weights;
    
    double value;
};

class InovDescriptor {
    public:
        int currentMax = 0;

        std::deque<inovDescriptor> globalInovDescriptorTable;

        InovExists getInov(inovDescriptor* inov) {
            for (int i = 0; i < globalInovDescriptorTable.size(); ++i) {
                if (globalInovDescriptorTable.at(i) >> *inov) {
                    return {globalInovDescriptorTable.at(i).innovation, true};
                }
            }
            
            ++currentMax;

            inov->innovation = currentMax;
            globalInovDescriptorTable.push_back(*inov);
            return {currentMax, false};
}
};

}; // namespace NEAT