#include <iostream>
#include <vector>
#include <string>
#include "Network.h"

int main()
{
    int input = 10;
    std::vector<int> layers = {9,8,7};
    int output = 6;
    std::vector<std::string> names = { "five", "four", "three", "two", "one", "zero" };
    Network n(input, layers, output, names);
    n.save("test.network.txt");
}