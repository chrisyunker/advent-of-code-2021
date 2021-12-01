#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <map>

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

    //std::map<int, int> m;
    int value = 10000;
    int count = 0;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        int n = std::stoi(line);
        if (n > value)
        {
            count++;
        }
        value = n;
    }

    printf("count: %d\n", count);


    return 0;
}
