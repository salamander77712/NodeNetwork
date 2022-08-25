#include "Network.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
Network::Network(int inputSize, std::vector<int> internalLayers, int outputSize, std::vector<std::string> outputNames) {
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
			previousLayerSize = internalLayers[i - 1];
		}
		for (int j = 0; j < internalLayers[i]; j++) {
			internalNodes[i].push_back(InternalNode(previousLayerSize));
		}
	}
	int previousLayerSize = internalLayers[internalLayers.size() - 1];
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
void Network::save(std::string file) {
	std::ofstream saveFile(file);
	saveFile << "Network" << "\n";
	saveFile << "Structure" << "\n";
	saveFile << "Inputs" << "\n";
	saveFile << num_inputs << "\n";
	saveFile << "Internal Layers" << "\n";
	saveFile << internalNodes.size() << "\n";
	for (int i = 0; i < internalNodes.size(); i++) {
		saveFile << "Layer " << i << " Size" << "\n";
		saveFile << internalNodes[i].size() << "\n";
	}
	saveFile << "Outputs" << "\n";
	saveFile << num_outputs << "\n";
	saveFile << "State" << "\n";
	for (int i = 0; i < internalNodes.size(); i++) {
		saveFile << "Layer " << i << "\n";
		saveFile << "Size" << "\n";
		saveFile << internalNodes[i].size() << "\n";
		for (int j = 0; j < internalNodes[i].size(); j++) {
			saveFile << "Internal Node " << j << "\n";
			saveFile << "Bias" << "\n";
			saveFile << internalNodes[i][j].getBias() << "\n";
			saveFile << "Weights" << "\n";
			saveFile << internalNodes[i][j].getWeights() << "\n";
		}
	}
	saveFile << "Output Nodes" << "\n";
	for (int i = 0; i < outputNodes.size(); i++) {
		saveFile << "Ouput Node" << "\n";
		saveFile << "Name" << "\n";
		saveFile << outputNodes[i].getName() << "\n";
		saveFile << "Bias" << "\n";
		saveFile << outputNodes[i].getBias() << "\n";
		saveFile << "Weights" << "\n";
		saveFile << outputNodes[i].getWeights() << "\n";
	}
	saveFile << "End";
	saveFile.close();
}