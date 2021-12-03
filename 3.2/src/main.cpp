#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

int main(int argc, char* argv[])
{
    const int numArgs = 1;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    std::string input(argv[optind]);

    printf("input: %s\n", input.c_str());

    std::vector<int> v;
    std::vector<std::string> ox;
    std::vector<std::string> co;
    int init = 0;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        if (!init)
        {
            for (int i = 0; i < line.size(); i++)
            {
                v.push_back(0);
            }
            init = 1;
        }

        for (std::string::size_type i = 0; i < line.size(); i++)
        {
            int val = (int)line[i];
            if (val == 48)
            {
                v[i]--;
            }
            else if (val == 49)
            {
                v[i]++;
            }
            else
            {
                printf("Bad value: %d\n", val);
                return 1;
            }

            ox.push_back(line);
            co.push_back(line);
        }
    }

    while (ox.size() > 1)
    {
        for (int i = 0; i < v.size(); i++)
        {
            int cmp = v[i] >= 0 : 49 ? 48;
            for (std::string e : ox)
            {
                if (e[i] != cmp)
                {

                }
            }
        }
    }


    std::string gammaStr = "";
    std::string epsilonStr = "";
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] > 0)
        {
            gammaStr += "1";
            epsilonStr += "0";
        }
        else
        {
            gammaStr += "0";
            epsilonStr += "1";
        }
    }
    printf("gamma: %s\n", gammaStr.c_str());
    printf("epsilon: %s\n", epsilonStr.c_str());

    int gamma = stoi(gammaStr, nullptr, 2);
    int epsilon = stoi(epsilonStr, nullptr, 2);

    printf("gamma: %d\n", gamma);
    printf("epsilon: %d\n", epsilon);

    printf("power: %d\n", gamma * epsilon);

    return 0;
}
