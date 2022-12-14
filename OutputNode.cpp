#include "OutputNode.h"
#include <string>
OutputNode::OutputNode(std::string nodeName, int inputSize) {
	num_inputs = inputSize;
	bias = 0;
	value = 0;
	name = nodeName;
	for (int i = 0; i < inputSize; i++) {
		weights.push_back((double)rand() / RAND_MAX);
	}
}
std::string OutputNode::getName() {
	return name;
}