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

    std::map<int, int> m;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        int n = std::stoi(line);

        //printf("%d\n", n);
        if (m.contains(2020 - n))
        {
            printf("found: %d and %d\n", (2020 - n), n);
            printf("product: %d\n", (2020 - n) * n);
        }
        else
        {
            m[n] = n;
        }
    }


    return 0;
}
