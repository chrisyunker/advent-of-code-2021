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

int simulate(Range r, Coord v, bool debug = false)
{
    Coord p = Coord{0, 0};
    int maxH = 0;

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

        if (p.y > maxH)
            maxH = p.y;

        if (p.x >= r.x1 &&
            p.x <= r.x2 &&
            p.y <= r.y1 &&
            p.y >= r.y2)
        {
            printf("success: loc: %d, %d, height: %d\n", p.x, p.y, maxH);
            return maxH;
        }
        if (p.x > r.x2 || p.y < r.y2)
        {
            printf("fail: loc: %d, %d\n", p.x, p.y);
            return 0;
        }
    } while (true);
}

int main(int argc, char *argv[])
{
    const int numArgs = 4;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    std::string input(argv[optind]);
    Coord vel;
    std::string type = argv[optind + 1];
    vel.x = std::stoi(argv[optind + 2]);
    vel.y = std::stoi(argv[optind + 3]);

    printf("Input file: %s\n", input.c_str());
    printf("Type: %s\n", type.c_str());
    printf("Max Init velocity: %d, %d\n", vel.x, vel.y);

    std::ifstream fd(input);
    Range r = ParseInput(fd);

    printf("%d  %d ---- %d\n", r.y1, r.x1, r.x2);
    printf(" |\n");
    printf(" |\n");
    printf("%d\n", r.y2);

    if (type == "m")
    {
        printf("Max Init velocity: %d, %d\n", vel.x, vel.y);

        int minx = 1;
        int miny = 1;
        int maxx = vel.x;
        int maxy = vel.y;

        int maxH = 0;
        Coord maxV;
        for (int x = minx; x < maxx; x++)
        {
            for (int y = miny; y < maxy; y++)
            {
                Coord v = Coord{x, y};
                int h = simulate(r, v);
                if (h > maxH)
                {
                    maxH = h;
                    maxV = v;
                }
            }
        }

        printf("Result: %d, vel: %d, %d\n", maxH, maxV.x, maxV.y);
    }
    else
    {
        printf("Single Init velocity: %d, %d\n", vel.x, vel.y);
        int h = simulate(r, vel, true);
        printf("Result: %d\n", h);
    }

    return 0;
}