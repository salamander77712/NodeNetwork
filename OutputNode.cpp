#include "OutputNode.h"
#include <string>
OutputNode::OutputNode(std::string nodeName, int inputSize) {
	name = nodeName;
	InternalNode::InternalNode(inputSize);
}
std::string OutputNode::getName() {
	return name;
}