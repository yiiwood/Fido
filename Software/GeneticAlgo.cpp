#include "GeneticAlgo.h"
using namespace gen;

GeneticAlgo::GeneticAlgo(int populationSize_, float mutationRate_, float crossoverRate_, double(*getFitness_)(net::NeuralNet neuralNet)) {
	populationSize = populationSize_;
	mutationRate = mutationRate_;
	crossoverRate = crossoverRate_;
	getFitness = getFitness_;
}

// Uses the crossoverRate variable to determine if two neural networks are mixed 
// and then and mixes their neurons weights to produce two new neural networks

void GeneticAlgo::crossover(net::NeuralNet mom, net::NeuralNet dad, net::NeuralNet& offspring1, net::NeuralNet& offspring2) {
	float crossoverDeterminer = rand() / RAND_MAX;
	if(crossoverDeterminer > crossoverRate) {
		offspring1 = mom;
		offspring2 = dad;
		return;
	}
	std::vector<double> offspring1Weights;
	std::vector<double> offspring2Weights;
	std::vector<double> momWeights = mom.getWeights();
	std::vector<double> dadWeights = dad.getWeights();

	// Crossover index must be a minimum of 1 and a maxiumum of the second to last index of the weights
	int crossoverIndex = (rand() % momWeights.size() - 2) + 1;

	for(int a = 0; a < crossoverIndex; a++) {
		offspring1Weights.push_back(momWeights[a]);
		offspring2Weights.push_back(dadWeights[a]);
	}
	for(int a = crossoverIndex; a < momWeights.size(); a++) {
		offspring1Weights.push_back(dadWeights[a]);
		offspring2Weights.push_back(momWeights[a]);
	}
	offspring1 = net::NeuralNet(mom.numInputs, mom.numOutputs, mom.numHiddenLayers, mom.numNeruronsPerHiddenLayer);
	offspring1.setWeights(offspring1Weights);
	offspring2 = net::NeuralNet(mom.numInputs, mom.numOutputs, mom.numHiddenLayers, mom.numNeruronsPerHiddenLayer);
	offspring2.setWeights(offspring2Weights);
}

// Uses the mutationRate variable to randomnly changes the weights of a neural network
net::NeuralNet GeneticAlgo::mutate(net::NeuralNet net) {
	std::vector<double> weights = net.getWeights();

	for(int a = 0; a < weights.size(); a++) {
		float mutationDeterminer = rand() / RAND_MAX;
		if(mutationDeterminer <= mutationRate) {
			weights[a] = net::Neuron::randomWeight();
		}
	}
}
