#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

struct Coord
{
    int x;
    int y;
};

struct Range
{
    int x1;
    int x2;
    int y1;
    int y2;
};

Range ParseInput(std::ifstream &fd)
{
    Range r;

    std::string line;
    std::getline(fd, line);

    size_t pos, next;
    pos = line.find_first_of("=", 0);
    pos++;
    next = line.find_first_of(".", pos);

    int x1 = std::stoi(line.substr(pos, next - pos));
    pos = pos + (next - pos);

    pos = line.find_first_not_of(".", pos);
    next = line.find_first_of(",", pos);
    int x2 = std::stoi(line.substr(pos, next - pos));
    pos = pos + (next - pos);

    pos = line.find_first_of("=", pos);
    pos++;
    next = line.find_first_of(".", pos);
    int y1 = std::stoi(line.substr(pos, next - pos));
    pos = pos + (next - pos);

    pos = line.find_first_not_of(".", pos);
    next = line.find_first_of(",", pos);
    int y2 = std::stoi(line.substr(pos, next - pos));
    pos = pos + (next - pos);

    if (x2 > x1)
    {
        r.x1 = x1;
        r.x2 = x2;
    }
    else
    {
        r.x1 = x2;
        r.x2 = x1;
    }

    if (y2 < y1)
    {
        r.y1 = y1;
        r.y2 = y2;
    }
    else
    {
        r.y1 = y2;
        r.y2 = y1;
    }

    return r;
}

bool simulate(Range r, Coord v, bool debug = false)
{
    Coord p = Coord{0, 0};

    do
    {
        if (debug)
            printf("loc: %d, %d, vel: %d, %d\n", p.x, p.y, v.x, v.y);

        p.x += v.x;
        p.y += v.y;
        if (v.x > 0)
            v.x -= 1;
        else if (v.x < 0)
            v.x += 1;
        v.y -= 1;

        if (p.x >= r.x1 &&
            p.x <= r.x2 &&
            p.y <= r.y1 &&
            p.y >= r.y2)
        {
            return true;
        }
        if (p.x > r.x2 || p.y < r.y2)
        {
            return false;
        }
    } while (true);
}

int main(int argc, char *argv[])
{
    const int numArgs = 3;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    std::string input(argv[optind]);
    int maxx = std::stoi(argv[optind + 1]);
    int miny = std::stoi(argv[optind + 2]);
    int maxy = std::stoi(argv[optind + 3]);

    printf("Input file: %s\n", input.c_str());
    printf("Init velocity range x: 1 - %d, y: %d - %d\n", maxx, miny, maxy);

    std::ifstream fd(input);
    Range r = ParseInput(fd);

    std::vector<Coord> vels;

    printf("Target Range:\n");
    printf("%d  %d ---- %d\n\n", r.y1, r.x1, r.x2);
    printf(" |\n");
    printf(" |\n\n");
    printf("%d\n", r.y2);

    int minx = 1;

    for (int x = minx; x < maxx; x++)
    {
        for (int y = miny; y < maxy; y++)
        {
            Coord v = Coord{x, y};
            if (simulate(r, v))
                vels.push_back(v);   
        }
    }

    for (auto v : vels)
    {
        printf("Valid init velocity: %d, %d\n", v.x, v.y);
    }
    printf("Total valid init velocities: %lu\n", vels.size());

    return 0;
}