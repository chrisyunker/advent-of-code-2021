#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <math.h>

std::vector<int> parseInput(std::ifstream &fd)
{
    std::vector<int> numbers;

    const char del = ',';

    std::string line;
    while (std::getline(fd, line))
    {

        size_t start;
        size_t end = 0;
        while ((start = line.find_first_not_of(del, end)) != std::string::npos)
        {
            end = line.find(del, start);
            numbers.push_back(std::stoi(line.substr(start, end - start)));
        }

    }
    return numbers;
}

int main(int argc, char* argv[])
{
    const int numArgs = 1;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    std::string input(argv[optind]);

    printf("input file: %s\n", input.c_str());

    std::ifstream fd(input);
   
    auto numbers = parseInput(fd);

    std::sort(numbers.begin(), numbers.begin()+numbers.size());

    int median = numbers[numbers.size()/2];

    int fuel = 0;
    for (auto n : numbers)
    {
        fuel += abs(median - n);
    }

    printf("position: %d, fuel: %d\n", median, fuel);

    return 0;
}
