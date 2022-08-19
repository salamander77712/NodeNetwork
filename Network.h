#pragma once
#include <vector>
#include <string>
#include "InputNode.h"
#include "InternalNode.h"
#include "OutputNode.h"
class Network {
private:
	int num_inputs;
	int num_outputs;
	std::vector<InputNode> inputNodes;
	std::vector<std::vector<InternalNode>> internalNodes;
	std::vector<OutputNode> outputNodes;
	void inputValues(std::vector<double> newInputs);
	void calculate();
	void learn(std::string correctValue);
public:
	Network(int inputSize, std::vector<std::vector<int>> internalLayers, int outputSize, std::vector<std::string> outputNames);
	std::string compute(std::vector<double> data);
	std::string computeAndLearn(std::vector<double> data, std::string correctValue);
};