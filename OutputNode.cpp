#include "OutputNode.h"
#include <string>
OutputNode::OutputNode(std::string nodeName, int inputSize) {
	num_inputs = inputSize;
	bias = 0;
	value = 0;
	name = nodeName;
}
std::string OutputNode::getName() {
	return name;
}