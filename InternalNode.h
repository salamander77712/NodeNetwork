#pragma once
#include <vector>
#include <string>
class InternalNode {
protected:
	double value;
	double bias;
	int num_inputs;
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
	std::vector<double> learn(double expectedValue, double speedPercent);
	double _calculateWithChange(int weightIndex, double newWeightValue);
	double _calculateWithChange(std::vector<double> newWeights);
	double _calculateWithChangeI(int inputIndex, double newInputValue);
	std::vector<double> weights;
};