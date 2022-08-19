#include "InternalNode.h"
#include <vector>
#include <stdexcept>
#include <math.h>
InternalNode::InternalNode(int inputSize) {
	num_inputs = inputSize;
	bias = 0;
	value = 0;
}
double sigmoid(double n) {
	return (1 / (1 + exp(-n));
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