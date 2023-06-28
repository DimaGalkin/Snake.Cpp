#include "genotype.hpp"

NEAT::Genome::Genome(NEAT::InovDescriptor* GIDTptr) {
    globalInovDescriptorTable = GIDTptr;
}

int NEAT::Genome::random() {
    return rand() % 2; // 0 or 1
}

int NEAT::Genome::minMaxRandom(int min, int max) { // inclusive
    return rand() % max + min;
}

double NEAT::Genome::minMaxFLPR(double min, double max) {
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

bool NEAT::Genome::contains(std::deque<ConnectionGene>* parent, int inovNum) {
    for (int i = 0; i < parent->size(); ++i) {
        if (parent->at(i).innovation == inovNum) {
            return true;
        }
    }

    return false;
}

void NEAT::Genome::combine(Genome* parent1, Genome* parent2) {
    Genome* fit;
    Genome* unfit;

    if (parent1->fitness > parent2->fitness) {
        fit = parent1;
        unfit = parent2;
    } else {
        fit = parent2;
        unfit = parent1;
    }

    for (int i = 0; i < unfit->connections.size(); ++i) {
        if (contains(&fit->connections, unfit->connections.at(i).innovation)) {
            if (random() == 0) {
                connections.push_back(unfit->connections.at(i));
            } else {
                connections.push_back(fit->connections.at(i));
            }

            fit->connections.erase(std::remove(fit->connections.begin(), fit->connections.end(), fit->connections.at(i)), fit->connections.end());
        }
    }

    for (int i = 0; i < fit->connections.size(); ++i) {
        connections.push_back(fit->connections.at(i));
    }
}

void NEAT::Genome::addConnection(int in, int out, double weight, bool enabled) {
    inovDescriptor searchTemplate = {
        in,
        out,
        0
    };

    InovExists data = globalInovDescriptorTable->getInov(&searchTemplate);

    if (!data.exists) {
        ConnectionGene newConnection = {
            in,
            out,
            weight,
            data.innovation,
            enabled
        };

        connections.push_back(newConnection);
    }
}

void NEAT::Genome::addNode(int id, GeneType type) {
    NodeGene newNode = {
        id,
        type
    };

    nodes.push_back(newNode);

    ++maxNode;
}

void NEAT::Genome::mutateChangeConnection() {
    int randomConnection = minMaxRandom(0, connections.size() - 1);
    double newWeight = minMaxFLPR(-2, 2);

    connections.at(randomConnection).weight = newWeight;

    std::cout << "\n[MUTATION] Changed connection " << randomConnection << " to " << newWeight <<  "\n\n";;
}

void NEAT::Genome::mutateModifyConnection() {
    int randomConnection = minMaxRandom(0, connections.size() - 1);
    double modifier = minMaxFLPR(-1, 1);

    connections.at(randomConnection).weight = connections.at(randomConnection).weight * modifier;

    std::cout << "\n[MUTATION] Modified connection " << randomConnection << " by " << modifier <<  "\n\n";;
}

void NEAT::Genome::mutateNewConnection() {
    int conInNum = minMaxRandom(0, globalInovDescriptorTable->globalInovDescriptorTable.size() - 1);
    int conOutNum = minMaxRandom(0, globalInovDescriptorTable->globalInovDescriptorTable.size() - 1);
    
    inovDescriptor conIn = globalInovDescriptorTable->globalInovDescriptorTable.at(conInNum);
    inovDescriptor conOut = globalInovDescriptorTable->globalInovDescriptorTable.at(conOutNum);

    int in = conIn.in;
    int out = conOut.out;

    addConnection(in, out, minMaxFLPR(-2, 2), true);

    std::cout << "\n[MUTATION] Added connection " << in << " to " << out <<  "\n\n";;
}

void NEAT::Genome::mutateNewNode() {
    int connectionNum = minMaxRandom(0, globalInovDescriptorTable->globalInovDescriptorTable.size());

    inovDescriptor connection = globalInovDescriptorTable->globalInovDescriptorTable.at(connectionNum);

    int in = connection.in;
    int out = connection.out;

    addNode(maxNode + 1, HIDDEN);

    addConnection(in, maxNode, minMaxFLPR(-2, 2), true);
    addConnection(maxNode, out, minMaxFLPR(-2, 2), true);

    std::cout << "\n[MUTATION] Added node " << maxNode << " between " << in << " and " << out <<  "\n\n";;
}

void NEAT::Genome::mutateEnableConnection() {
    int randomConnection = minMaxRandom(0, connections.size() - 1);
    
    if (connections.at(randomConnection).enabled) {
        connections.at(randomConnection).enabled = false;
    } else {
        connections.at(randomConnection).enabled = true;
    }

    std::cout << "\n[MUTATION] Toggled connection " << randomConnection << " to " << connections.at(randomConnection).enabled <<  "\n\n";
}

void NEAT::Genome::mutate() {
    int mutation = minMaxRandom(0, 5);

    switch (mutation) {
        case 0:
            mutateChangeConnection();
            break;
        case 1:
            mutateModifyConnection();
            break;
        case 2:
            mutateNewConnection();
            break;
        case 3:
            mutateNewNode();
            break;
        case 4:
            mutateEnableConnection();
            break;
        default:
            std::cout << "Error: Mutation not found" << std::endl;
            break;
    }
}

void NEAT::Genome::generatePhenotype() {
    network.reset();

    network = std::make_unique<NEAT::Phenotype>();
    network->generateNetwork(&connections, &nodes);
}

int NEAT::Genome::runGenome(std::unique_ptr<std::map<int, double>> values) {
    int neuronNum = std::move(network->execNetwork(std::move(values)));
    
    return neuronNum;
}