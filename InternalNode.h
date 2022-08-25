#pragma once
#include <vector>
#include <string>
class InternalNode {
protected:
	double value;
	double bias;
	int num_inputs;
	std::vector<double> weights;
	std::vector<double> inputs;
	double sigmoid(double n);
public:
	InternalNode();
	InternalNode(int inputSize);
	double getValue();
	double getBias();
	std::string getWeights();
	void setInputs(std::vector<double> newInputs);
	void calculateValue();
};