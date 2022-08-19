#pragma once
#include "InternalNode.h"
#include <string>
class OutputNode : public InternalNode {
private:
	std::string name;
public:
	OutputNode(std::string nodeName, int inputSize);
	std::string getName();
};