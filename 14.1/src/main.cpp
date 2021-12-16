#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <list>
#include <map>


void parseInput(std::ifstream &fd, std::list<char> &polymer, std::map<std::string, char>& rules)
{
    std::string line;
    std::getline(fd, line);
    for (auto c : line)
    {
        polymer.push_back(c);
    }
    std::getline(fd, line);

    while (std::getline(fd, line))
    {
        rules[line.substr(0, 2)] = line[6];
    }
};

void printPolymer(std::list<char>& polymer)
{
    printf("Polymer: ");
    for (auto c : polymer)
    {
        printf("%c", c);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    const int numArgs = 2;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    std::string input(argv[optind]);
    int steps = std::stoi(argv[optind+1]);

    printf("Input file: %s\n", input.c_str());
    printf("Steps: %d\n", steps);

    std::ifstream fd(input);

    std::map<std::string, char> rules;
    std::list<char> polymer;

    parseInput(fd, polymer, rules);

    printPolymer(polymer);

    for (int i = 0; i < steps; i++)
    {
        std::list<char>::iterator it;
        for (it = polymer.begin(); it != polymer.end(); it++)
        {
            if (std::next(it) != polymer.end())
            {
                std::string key{*it, *std::next(it)};
                if (rules.contains(key))
                {
                    polymer.insert(std::next(it), rules[key]);
                    it++;
                }
            }
        }
        printf("Steps: %d\n", i);
        //printPolymer(polymer);
    }

    std::map<char, u_long> count;
    for (auto c : polymer)
    {
        if (count.contains(c))
        {
            count[c]++;
        }
        else
        {
            count[c] = 1;
        }
    }

    u_long maxV = 0;
    unsigned long minV = 0;
    char maxC, minC;
    for (auto [k,v] : count)
    {
        if (v > maxV)
        {
            maxV = v;
            maxC = k;
        }
        if (minV == 0 || v < minV)
        {
            minV = v;
            minC = k;
        }
    }

    printf("max: %c, %lu, min: %c, %lu\n", maxC, maxV, minC, minV);
    printf("max - min: %lu\n", (maxV - minV));

    return 0;
}
