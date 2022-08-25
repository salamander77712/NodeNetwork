#include "InternalNode.h"
#include <vector>
#include <stdexcept>
#include <math.h>
#include <string>
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
		weights.push_back(0);
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