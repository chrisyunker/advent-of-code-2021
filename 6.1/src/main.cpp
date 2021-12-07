#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
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
    Fish(int t) : timer{t}
    {
    }

    bool IncrementDay()
    {
        if (timer == 0)
        {
            timer = 6;
            return true;
        }
        timer -= 1;
        return false;
    }

    int timer;
};

void printFish(int day, const std::vector<Fish>& fish)
{
    printf("Day: %d : ", day);
    for (const Fish& f: fish)
    {
        printf("%d ", f.timer);
    }
    printf("\n");
}

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
    std::vector<Fish> fish;

    printf("Number of numbers: %lu\n", numbers.size());

    for (auto n : numbers)
    {
        Fish f{ n };
        fish.push_back(f);
    }

    for (int i = 0; i < days; i++)
    {
        //printf("Day: %d\n", i);

        std::vector<Fish> newFish;
        for (Fish& f : fish)
        {
            if (f.IncrementDay())
            {
                //printf("New Fish\n");
                Fish nf{ 8 };
                newFish.push_back(std::move(nf));
            }
        }
        for (const Fish& f : newFish)
        {
            fish.push_back(f);
        }
        printFish(i, fish);
    }

    

    printf("Number of fish: %lu\n", fish.size());

    return 0;
}
