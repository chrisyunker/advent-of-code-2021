#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <numeric>

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

long calcFuel(int location, std::vector<int>& numbers)
{
    long fuel = 0L;
    for (auto n : numbers)
    {
        int d = abs(location - n);
        fuel += d * (d + 1)/2;
    }
    return fuel;
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

    int avg = round(std::accumulate(numbers.begin(), numbers.end(), 0.0) / numbers.size());
    
    long fuel = calcFuel(avg, numbers);
    printf("position: %d, fuel: %ld\n", avg, fuel);

    long fuel_m1 = calcFuel(avg-1, numbers);
    printf("position: %d, fuel: %ld\n", avg-1, fuel_m1);

    long fuel_p1 = calcFuel(avg+1, numbers);
    printf("position: %d, fuel: %ld\n", avg+1, fuel_p1);

    return 0;
}
