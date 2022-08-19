#pragma once
#include <vector>
class InternalNode {
protected:
	double value;
	double bias;
	int num_inputs;
	std::vector<double> weights;
	std::vector<double> inputs;
	double sigmoid(double n);
public:
	InternalNode(int inputSize);
	double getValue();
	void setInputs(std::vector<double> newInputs);
	void calculateValue();
};