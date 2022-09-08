#include <iostream>
#include <vector>
#include <string>
#include "Network.h"
struct Data {
    std::vector<std::vector<double>> datas;
    std::vector<std::string> answers;
};
Data generateData(int number) {
    Data output;
    output.datas = {};
    output.answers = {};
    for (int i = 0; i < number; i++) {
        double ones = rand() % 2;
        double twos = rand() % 2;
        output.datas.push_back({ twos, ones });
        if (twos == 0 && ones == 0) {
            output.answers.push_back("0");
        }
        else if (twos == 0 && ones == 1) {
            output.answers.push_back("1");
        }
        else if (twos == 1 && ones == 0) {
            output.answers.push_back("2");
        }
        else if (twos == 1 && ones == 1) {
            output.answers.push_back("3");
        }
    }
    return output;
}
int main()
{
    int input = 2;
    std::vector<int> layers = {4};
    std::vector<std::string> names = { "0", "1", "2", "3"};
    int output = names.size();
    Network n(input, layers, output, names);
    n.save("before.nextwork.txt");
    int num_wrong = 0;
    int num_right = 0;

    Data d = generateData(100);
    std::vector<std::string> results = n.computeAndLearnMass(d.datas, d.answers);
    for (int i = 0; i < results.size(); i++) {
        if (d.answers[i] == results[i]) {
            num_right++;
        }
        else {
            num_wrong++;
        }
        std::cout << "Expected: " << d.answers[i] << " Got: " << results[i] << std::endl;
    }
    std::cout << "<------Data-After-Training------>" << std::endl;
    d = generateData(100);
    results = n.computeAndLearnMass(d.datas, d.answers);
    for (int i = 0; i < results.size(); i++) {
        if (d.answers[i] == results[i]) {
            num_right++;
        }
        else {
            num_wrong++;
        }
        std::cout << "Expected: " << d.answers[i] << " Got: " << results[i] << std::endl;
    }
    d = generateData(100);
    results = n.computeAndLearnMass(d.datas, d.answers);
    for (int i = 0; i < results.size(); i++) {
        if (d.answers[i] == results[i]) {
            num_right++;
        }
        else {
            num_wrong++;
        }
        std::cout << "Expected: " << d.answers[i] << " Got: " << results[i] << std::endl;
    }

    std::cout << "Number Wrong: " << num_wrong << std::endl;
    std::cout << "Number Right: " << num_right << std::endl;
    n.save("after.network.txt");
}