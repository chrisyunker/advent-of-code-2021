#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

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

struct Fish
{
    long number = 0L;
    long pending = 0L;
};

int main(int argc, char* argv[])
{
    const int numArgs = 2;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    std::string input(argv[optind]);
    int days = std::stoi(argv[optind + 1]);

    printf("input file: %s\n", input.c_str());
    printf("input days: %d\n", days);

    std::ifstream fd(input);
   
    auto numbers = parseInput(fd);

    printf("Number of numbers: %lu\n", numbers.size());

    Fish rb[7];

    for (auto n : numbers)
    {
        rb[n % 7].number += 1;
    }

    for (int i = 0; i < days; i++)
    {
        rb[(i + 2) % 7].pending += rb[i % 7].number;
        rb[i % 7].number += rb[i % 7].pending;
        rb[i % 7].pending = 0;
    }

    long total = 0;
    for (const Fish& fish : rb)
    {
        printf("Number of fish: %lu + %lu\n", fish.number, fish.pending);

        total += fish.number + fish.pending;
    }
    printf("Number of fish: %lu\n", total);

    return 0;
}
