#include "InternalNode.h"
#include <vector>
#include <stdexcept>
#include <math.h>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
InternalNode::InternalNode() {
	num_inputs = 0;
	bias = 0;
	value = 0;
}
InternalNode::InternalNode(int inputSize) {
	num_inputs = inputSize;
	bias = 0;
	value = 0;
	for (int i = 0; i < inputSize; i++) {
		weights.push_back((double)rand() / RAND_MAX);
	}
}
double InternalNode::sigmoid(double n) {
	return (1 / (1 + exp(-n)));
}
double InternalNode::getValue() {
	return value;
}
void InternalNode::setInputs(std::vector<double>newInputs) {
	if (newInputs.size() != num_inputs) {
		throw std::invalid_argument("setInputs vector length must be the same as num_inputs");
	}
	inputs = newInputs;
}
void InternalNode::calculateValue() {
	value = bias;
	for (int i = 0; i < inputs.size(); i++) {
		value += inputs[i] * weights[i];
	}
	value = sigmoid(value);
}
double InternalNode::getBias() {
	return bias;
}
std::string InternalNode::getWeights() {
	std::string output = "{";
	for (int i = 0; i < weights.size(); i++) {
		output += std::to_string(weights[i]);
		if (i != weights.size() - 1) {
			output += ",";
		}
	}
	output += "}";
	return output;
}
double InternalNode::_calculateWithChange(int weightIndex, double newWeightValue) {
	double _value = bias;
	for (int i = 0; i < inputs.size(); i++) {
		if (i == weightIndex) {
			_value += inputs[i] * newWeightValue;
		}
		else {
			_value += inputs[i] * weights[i];
		}
	}
	return sigmoid(_value);
}
double InternalNode::_calculateWithChange(std::vector<double> newWeights) {
	double _value = bias;
	for (int i = 0; i < inputs.size(); i++) {
		_value += inputs[i] * newWeights[i];
	}
	return sigmoid(_value);
}
double InternalNode::_calculateWithChangeI(int inputIndex, double newInputValue) {
	double _value = bias;
	for (int i = 0; i < inputs.size(); i++) {
		if (i == inputIndex) {
			_value += newInputValue * weights[i];
		}
		else {
			_value += inputs[i] * weights[i];
		}
	}
	return sigmoid(_value);
}
std::vector<double> InternalNode::learn(double expectedValue, double speedPercent) {
	double diff = abs(expectedValue - value);
	std::vector<double> weightOffsets;
	for (int i = 0; i < weights.size(); i++) {
		double valueLess = _calculateWithChange(i, weights[i] - weights[i] * speedPercent);
		double valuePlus = _calculateWithChange(i, weights[i] + weights[i] * speedPercent);
		double diffLess = abs(valueLess - expectedValue);
		double diffPlus = abs(valuePlus - expectedValue);
		double offset = 0;
		//std::cout << value << " " << valueLess << " " << valuePlus << std::endl;
		//std::cout << diff << " " << diffLess << " " << diffPlus << std::endl;
		if (diffLess < diff) {
			offset = -1 * weights[i] * speedPercent;
		}
		if (diffPlus < diff && diffPlus < diffLess) {
			offset = weights[i] * speedPercent;
		}
		weightOffsets.push_back(offset);
	}
	std::vector<double> newWeights;
	for (int i = 0; i < weights.size(); i++) {
		newWeights.push_back(weights[i] + weightOffsets[i]);
	}
	double learnedValue = _calculateWithChange(newWeights);
	double learnedDiff = abs(learnedValue - expectedValue);
	double imporvement = diff - learnedDiff;
	if (imporvement >= 0) {
		weights = newWeights;
	}
	std::vector<double> output;
	for (int i = 0; i < num_inputs; i++) {
		double valueLess = _calculateWithChangeI(i, inputs[i] - inputs[i] * speedPercent);
		double valuePlus = _calculateWithChangeI(i, inputs[i] + inputs[i] * speedPercent);
		double diffLess = abs(valueLess - expectedValue);
		double diffPlus = abs(valuePlus - expectedValue);
		double wantedValue = inputs[i];
		if (diffLess < learnedDiff) {
			wantedValue = inputs[i] - inputs[i] * speedPercent;
		}
		if (diffPlus < learnedDiff && diffPlus < diffLess) {
			wantedValue = inputs[i] + inputs[i] * speedPercent;
		}
		output.push_back(wantedValue);
	}
	return output;
}