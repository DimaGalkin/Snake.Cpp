#pragma once

#include <map>
#include <time.h>
#include <memory>
#include <algorithm>

#include "../utils.hpp"
#include "../phenotype/phenotype.hpp"

namespace NEAT {

class Genome {
    public:
        double fitness;
        int maxNode = 0;
        NEAT::InovDescriptor* globalInovDescriptorTable;
        std::deque<NEAT::ConnectionGene> connections;
        std::deque<NEAT::NodeGene> nodes;

        std::unique_ptr<NEAT::Phenotype> network;

        Genome(NEAT::InovDescriptor* GIDTptr);

        int random();

        int minMaxRandom(int min, int max);
        double minMaxFLPR(double min, double max);

        bool contains(std::deque<NEAT::ConnectionGene>* parent, int inovNum);

        void combine(Genome* parent1, Genome* parent2);

        void addConnection(int in, int out, double weight, bool enabled);

        void addNode(int id, NEAT::GeneType type);

        void mutateChangeConnection();
        void mutateModifyConnection();
        void mutateNewConnection();
        void mutateNewNode();
        void mutateEnableConnection();
        void mutate();

        void generatePhenotype();

        int runGenome(std::unique_ptr<std::map<int, double>> values);
};

};