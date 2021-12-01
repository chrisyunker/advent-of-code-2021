#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>

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

    int count = 0;
    std::vector<int> l;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        int n = std::stoi(line);
        l.push_back(n);
        int s = l.size();
        if (s < 4) continue;
        
        int prev = l[s-2] + l[s-3] + l[s-4];
        int next = l[s-1] + l[s-2] + l[s-3];

        if (next > prev)
        {
            count++;
        }
    }

    printf("count: %d\n", count);


    return 0;
}
