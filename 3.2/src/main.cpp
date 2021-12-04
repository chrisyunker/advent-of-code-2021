#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

int majority(std::vector<std::string> v, int idx)
{
    int acc = 0;
    for (auto item : v)
    {
        int val = (int)item[idx];
        if (val == 48)
            acc--;
        else if (val == 49)
            acc++;
    }
    return acc;
}

std::vector<std::string> filter(std::vector<std::string> v, int idx, int cmp)
{
    std::vector<std::string> out;

    for (int i = 0; i < v.size(); i++)
    {
        if (v[i][idx] == cmp)
        {
            out.push_back(v[i]);
        }
    }

    return out;
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

    printf("input: %s\n", input.c_str());

    std::vector<std::string> ox;
    std::vector<std::string> co;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        ox.push_back(line);
        co.push_back(line);
    }

    int idx = 0;
    while (ox.size() > 1)
    {
        int cmp = majority(ox, idx) >= 0 ? 49 : 48;
        ox = filter(ox, idx, cmp);
        idx++;
    }


    idx = 0;
    while (co.size() > 1)
    {
        int cmp = majority(co, idx) >= 0 ? 48 : 49;
        co = filter(co, idx, cmp);
        idx++;
    }

    printf("ox bin: %s\n", ox[0].c_str());
    printf("co bin: %s\n", co[0].c_str());

    int oxv = stoi(ox[0], nullptr, 2);
    int cov = stoi(co[0], nullptr, 2);

    printf("ox dec: %d\n", oxv);
    printf("co dec: %d\n", cov);

    printf("rating: %d\n", oxv * cov);


    return 0;
}