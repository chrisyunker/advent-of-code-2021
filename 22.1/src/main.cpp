#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

struct Cuboid
{
    bool state = false;
    int x1;
    int x2;
    int y1;
    int y2;
    int z1;
    int z2;   
};

bool grid[101][101][101] = {{{0}}};

void ParseInput(std::ifstream& fd, std::vector<Cuboid>& cuboids)
{
    std::string line;
    size_t pos, next = 0;
    while(std::getline(fd, line))
    {
        Cuboid c;
        pos = line.find_first_of(' ', 0);
        if (line.substr(0, pos) == "on")
            c.state = true;
        else
            c.state = false;

        pos = line.find_first_of('=', pos);
        pos++;
        next = line.find_first_of('.', pos);
        c.x1 = std::stoi(line.substr(pos, next-pos));
        pos = next + 2;
        next = line.find_first_of(',', pos);
        c.x2 = std::stoi(line.substr(pos, next - pos));

        pos = line.find_first_of('=', pos);
        pos++;
        next = line.find_first_of('.', pos);
        c.y1 = std::stoi(line.substr(pos, next-pos));
        pos = next + 2;
        next = line.find_first_of(',', pos);
        c.y2 = std::stoi(line.substr(pos, next - pos));

        pos = line.find_first_of('=', pos);
        pos++;
        next = line.find_first_of('.', pos);
        c.z1 = std::stoi(line.substr(pos, next-pos));
        pos = next + 2;
        next = line.find_first_of(',', pos);
        c.z2 = std::stoi(line.substr(pos, next - pos));

        cuboids.push_back(c);
    }
}

int main(int argc, char *argv[])
{
    const int numArgs = 1;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    std::string input(argv[optind]);
    printf("Input file: %s\n", input.c_str());

    std::vector<Cuboid> cuboids;

    std::ifstream fd(input);
    ParseInput(fd, cuboids);

    int on = 0;

    for (Cuboid& c : cuboids)
    {
        printf("Cuboid, state:%d, x:%d-%d, y:%d-%d, z:%d-%d\n",
               c.state, c.x1, c.x2, c.y1, c.y2, c.z1, c.z2);

        for (int x = std::max(c.x1, -50); x <= std::min(c.x2, 50); x++)
        {
            for (int y = std::max(c.y1, -50); y <= std::min(c.y2, 50); y++)
            {
                for (int z = std::max(c.z1, -50); z <= std::min(c.z2, 50); z++)
                {
                    //printf("Set: %d, %d, %d\n", x, y, z);

                    if (grid[x+50][y+50][z+50] == false && c.state == true)
                    {
                        on++;
                    }
                    else if (grid[x+50][y+50][z+50] == true &&  c.state == false)
                    {
                        on--;
                    }
                    grid[x+50][y+50][z+50] = c.state;
                }
            }
        }
    }

    printf("Total cubes on: %d\n", on);

    return 0;
}