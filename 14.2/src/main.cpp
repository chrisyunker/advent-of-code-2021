#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <array>

void incCount(char c, std::map<char, uint64_t>& count, uint64_t value)
{
    if (count.contains(c))
        count[c] += value;
    else
        count[c] = value;
}

void parseInput(std::ifstream &fd,
                std::map<std::string, uint64_t> &polMap,
                std::map<char, uint64_t> &count,
                std::map<std::string, char> &rules)
{
    std::string line;
    std::getline(fd, line);

    for (int i = 0; i < line.size() - 1; i++)
    {
        std::string key{line.substr(i, 2)};
        incCount(line[i], count, 1);
        if (polMap.contains(key))
            polMap[key] += 1;
        else
            polMap[key] = 1;
    }
    incCount(line[line.size()-1], count, 1);

    std::getline(fd, line);

    while (std::getline(fd, line))
    {
        rules[line.substr(0, 2)] = line[6];
    }
};

void doStep(std::map<std::string, uint64_t> &polMap,
            std::map<char, uint64_t> &count,
            std::map<std::string, char> &rules)
{
    std::map<std::string, uint64_t> polMap2;

    for (auto& [k, v] : polMap)
    {
        char c = rules[k];

        incCount(c, count, v);
        std::string k1{ k[0], c };
        std::string k2{ c, k[1] };
        
        if (polMap2.contains(k1))
            polMap2[k1] += v;
        else
            polMap2[k1] = v;

        if (polMap2.contains(k2))
            polMap2[k2] += v;
        else
            polMap2[k2] = v;
    }

    polMap = polMap2;
}

void printPolMap(std::map<std::string, uint64_t>& polMap)
{
    for (auto& [k, v] : polMap)
    {
        printf("%s : %llu\n", k.c_str(), v);
    }
}

void countLetters(std::map<char, uint64_t>& count)
{
    uint64_t maxV = 0;
    uint64_t minV = 0;
    char maxC, minC;

    for (auto& [k,v] : count)
    {
        printf("%c : %llu\n", k, v);
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

    printf("max: %c, %llu, min: %c, %llu\n", maxC, maxV, minC, minV);
    printf("max - min: %llu\n", (maxV - minV));
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
    std::map<std::string, uint64_t> polMap;
    std::map<char, uint64_t> count;

    parseInput(fd, polMap, count, rules);
    
    for (int i = 1; i <= steps; i++)
    {
        doStep(polMap, count, rules);
        printf("Step: %d\n", i);
        //printPolMap(pm);
        countLetters(count);
    }

    return 0;
}
