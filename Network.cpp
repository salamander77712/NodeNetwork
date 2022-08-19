#include "Network.h"
#include <vector>
#include <string>
Network::Network(int inputSize, std::vector<std::vector<int>> internalLayers, int outputSize, std::vector<std::string> outputNames) {
	num_inputs = inputSize;
	num_outputs = outputSize;
	for (int i = 0; i < num_inputs; i++) {
		inputNodes.push_back(InputNode());
	}
	for (int i = 0; i < internalLayers.size(); i++) {
		internalNodes.push_back(std::vector<InternalNode>{});
		int previousLayerSize;
		if (i == 0) {
			previousLayerSize = inputSize;
		}
		else{
			previousLayerSize = internalLayers[i - 1].size();
		}
		for (int j = 0; j < internalLayers[i].size(); j++) {
			internalNodes[i].push_back(InternalNode(previousLayerSize));
		}
	}
	int previousLayerSize = internalLayers[internalLayers.size() - 1].size();
	for (int i = 0; i < num_outputs; i++) {
		outputNodes.push_back(OutputNode(outputNames[i], previousLayerSize));
	}
}
void Network::inputValues(std::vector<double> newInputs) {
	for (int i = 0; i < inputNodes.size(); i++) {
		inputNodes[i].setValue(newInputs[i]);
	}
}
void Network::calculate() {
	std::vector<double> initialValuesToInsert;
	for (int i = 0; i < inputNodes.size(); i++) {
		initialValuesToInsert.push_back(inputNodes[i].getValue());
	}
	std::vector<double> nextValuesToInsert;
	std::vector<double> valuesToInsert;
	for (int i = 0; i < internalNodes.size(); i++) {
		if (i == 0) {
			valuesToInsert = initialValuesToInsert;
		}
		else {
			valuesToInsert = nextValuesToInsert;
		}
		for (int j = 0; j < internalNodes[i].size(); j++) {
			internalNodes[i][j].setInputs(valuesToInsert);
			internalNodes[i][j].calculateValue();
		}
		nextValuesToInsert = {};
		for (int j = 0; j < internalNodes[i].size(); j++) {
			nextValuesToInsert.push_back(internalNodes[i][j].getValue());
		}
	}
	for (int i = 0; i < num_outputs; i++) {
		outputNodes[i].setInputs(nextValuesToInsert);
		outputNodes[i].calculateValue();
	}
}
void Network::learn(std::string correctValue) {
	//TODO
	return;
}
std::string Network::compute(std::vector<double> data) {
	inputValues(data);
	calculate();
	double max = 0;
	int maxIndex = 0;
	for (int i = 0; i < num_outputs; i++) {
		if (outputNodes[i].getValue() > max) {
			max = outputNodes[i].getValue();
			maxIndex = i;
		}
	}
	return outputNodes[maxIndex].getName();
}
std::string Network::computeAndLearn(std::vector<double> data, std::string correctValue) {
	std::string output = compute(data);
	learn(correctValue);
	return output;
}