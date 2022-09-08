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
	std::vector<std::vector<double>> wantedInputs;
	double cost = 0;
	for (int i = 0; i < num_outputs; i++) {
		double expectedValue = 0;
		if (outputNodes[i].getName() == correctValue) {
			expectedValue = 1;
		}
		cost += (outputNodes[i].getValue() - expectedValue) * (outputNodes[i].getValue() - expectedValue);
	}
	for (int i = 0; i < num_outputs; i++) {
		double expectedValue = 0;
		if (outputNodes[i].getName() == correctValue) {
			expectedValue = 1;
		}
		wantedInputs.push_back(outputNodes[i].learn(expectedValue, .1));
	}
	for (int i = 0; i < internalNodes.size(); i++) {
		std::vector<std::vector<double>> _wantedInputs;
		for (int j = 0; j < internalNodes[i].size(); j++) {
			double expectedValue = 0;
			for (int k = 0; k < wantedInputs.size(); k++) {
				expectedValue += wantedInputs[k][j];
			}
			expectedValue /= wantedInputs.size();
			_wantedInputs.push_back(internalNodes[i][j].learn(expectedValue, .1));
		}
		wantedInputs = _wantedInputs;
	}
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
std::vector<std::string> Network::computeAndLearnMass(std::vector<std::vector<double>> datas, std::vector<std::string> correctValues) {
	std::vector<std::string> output;
	for (int i = 0; i < datas.size(); i++) {
		output.push_back(compute(datas[i]));
	}
	learnMass(datas, correctValues);
	return output;
}
void Network::learnMass(std::vector<std::vector<double>> datas, std::vector<std::string> correctValues) {
	double cost = 0;
	for (int i = 0; i < datas.size(); i++) {
		compute(datas[i]);
		for (int j = 0; j < num_outputs; j++) {
			double expectedValue = 0;
			if (outputNodes[j].getName() == correctValues[i]) {
				expectedValue = 1;
			}
			cost += (outputNodes[j].getValue() - expectedValue) * (outputNodes[j].getValue() - expectedValue);
		}
	}
	for (int m = 0; m < datas.size(); m++) {
		for (int i = 0; i < num_outputs; i++) {
			for (int j = 0; j < outputNodes[i].weights.size(); j++) {
				double startingWeight = outputNodes[i].weights[j];
				outputNodes[i].weights[j] += .1;
				double costPlus = 0;
				for (int k = 0; k < datas.size() && k <= m; k++) {
					compute(datas[k]);
					for (int l = 0; l < num_outputs; l++) {
						double expectedValue = 0;
						if (outputNodes[l].getName() == correctValues[k]) {
							expectedValue = 1;
						}
						costPlus += (outputNodes[l].getValue() - expectedValue) * (outputNodes[l].getValue() - expectedValue);
					}
				}
				outputNodes[i].weights[j] -= .2;
				double costLess = 0;
				for (int k = 0; k < datas.size() && k <= m; k++) {
					compute(datas[k]);
					for (int l = 0; l < num_outputs; l++) {
						double expectedValue = 0;
						if (outputNodes[l].getName() == correctValues[k]) {
							expectedValue = 1;
						}
						costLess += (outputNodes[l].getValue() - expectedValue) * (outputNodes[l].getValue() - expectedValue);
					}
				}
				if (costPlus < cost) {
					if (costPlus < costLess) {
						outputNodes[i].weights[j] += .2;
						//std::cout << "Weight Increased" << std::endl;
					}
				}
				else if (cost < costLess) {
					outputNodes[i].weights[j] += .1;
					//std::cout << "Weight Static" << std::endl;
				}
				//std::cout << "Weight Decreased" << std::endl;
			}
		}
	}
}
void Network::save(std::string file) {
	std::ofstream saveFile(file);
	saveFile << "Network" << std::endl;
	saveFile << "Structure" << std::endl;
	saveFile << "Inputs" << std::endl;
	saveFile << num_inputs << std::endl;
	saveFile << "Internal Layers" << std::endl;
	saveFile << internalNodes.size() << std::endl;
	for (int i = 0; i < internalNodes.size(); i++) {
		saveFile << "Layer " << i << " Size" << std::endl;
		saveFile << internalNodes[i].size() << std::endl;
	}
	saveFile << "Outputs" << std::endl;
	saveFile << num_outputs << std::endl;
	saveFile << "State" << std::endl;
	for (int i = 0; i < internalNodes.size(); i++) {
		saveFile << "Layer " << i << std::endl;
		saveFile << "Size" << std::endl;
		saveFile << internalNodes[i].size() << std::endl;
		for (int j = 0; j < internalNodes[i].size(); j++) {
			saveFile << "Internal Node " << j << std::endl;
			saveFile << "Bias" << std::endl;
			saveFile << internalNodes[i][j].getBias() << std::endl;
			saveFile << "Weights" << std::endl;
			saveFile << internalNodes[i][j].getWeights() << std::endl;
		}
	}
	saveFile << "Output Nodes" << std::endl;
	for (int i = 0; i < outputNodes.size(); i++) {
		saveFile << "Ouput Node" << std::endl;
		saveFile << "Name" << std::endl;
		saveFile << outputNodes[i].getName() << std::endl;
		saveFile << "Bias" << std::endl;
		saveFile << outputNodes[i].getBias() << std::endl;
		saveFile << "Weights" << std::endl;
		saveFile << outputNodes[i].getWeights() << std::endl;
	}
	saveFile << "End";
	saveFile.close();
}