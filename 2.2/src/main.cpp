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

    int x  = 0;
    int y = 0;
    int aim = 0;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        size_t start;
        size_t end = 0;
        std::vector<std::string> values;
        while ((start = line.find_first_not_of(' ', end)) != std::string::npos)
        {
            end = line.find(' ', start);
            values.push_back(line.substr(start, end - start));
        }

        std::string dir = values[0];
        int mag = std::stoi(values[1]);

        printf("dir: %s, mag: %d\n", dir.c_str(), mag);
        if (dir.compare("forward") == 0)
        {
            y += (aim * mag);
            x += mag;
        }
        else if (dir.compare("up") == 0)
        {
            //y -= mag;
            aim -= mag;
        }
        else if (dir.compare("down") == 0)
        {
            //y += mag;
            aim += mag;
        }
        printf("x: %d, y: %d, aim: %d\n", x, y, aim);
    }
    printf("x: %d, y: %d\n", x, y);
    printf("total: %d\n", x * y);


    return 0;
}
