#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <set>
#include <map>

struct Coord
{
    int x = 0;
    int y = 0;
};

struct Fold
{
    char type = 'z';
    int loc = 0;
};

struct Node
{
    bool on = false;
};

void parseInput(std::ifstream &fd, std::vector<Coord> &coords, std::vector<Fold> &folds, Coord &size)
{
    std::string del = "-";

    std::string line;
    while (std::getline(fd, line))
    {
        if (line == "")
            break;

        std::string del = ",";

        size_t pos;
        size_t prev = 0;
        int x = 0;
        int y = 0;
        while ((pos = line.find_first_of(del, prev)) != std::string::npos)
        {
            if (pos > prev)
            {
                x = std::stoi(line.substr(prev, pos - prev));
            }
            prev = pos + 1;
        }
        if (prev < line.length())
        {
            y = std::stoi(line.substr(prev, std::string::npos));
        }
        if (x > size.x)
            size.x = x;
        if (y > size.y)
            size.y = y;
        coords.push_back(Coord{x, y});
    }
    size.x++;
    size.y++;

    while (std::getline(fd, line))
    {
        size_t pos;
        char type = 'z';
        int loc;
        std::string del = "xy";
        while ((pos = line.find_first_of(del, 0)) != std::string::npos)
        {
            if (pos > 0)
            {
                type = line.substr(pos, 1)[0];
            }
            pos = pos + 2;
            break;
        }

        loc = std::stoi(line.substr(pos, std::string::npos));
        folds.push_back(Fold{type, loc});
    }
};

int fold(Fold f, Coord& size, Node **grid)
{
    int count = 0;
    if (f.type == 'y')
    {
        int ny = size.y / 2;

        for (int y = 0; y < ny; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                grid[x][y].on = grid[x][y].on || grid[x][size.y-1-y].on;
                if (grid[x][y].on)
                    count++;
            }
        }
        size.y = ny;
    }
    else if (f.type == 'x')
    {
        int nx = size.x / 2;

        for (int x = 0; x < nx; x++)
        {
            for (int y = 0; y < size.y; y++)
            {
                grid[x][y].on = grid[x][y].on || grid[size.x-1-x][y].on;
                if (grid[x][y].on)
                    count++;
            }
        }

        size.x = nx;
    }
    else
    {
        printf("Bad fold type: %c\n", f.type);
    }

    return count;
}


void printGrid(Coord s, Node **g)
{
    printf("\n");
    printf("Size: %d, %d\n", s.x, s.y);
    for (int y = 0; y < s.y; y++)
    {
        for (int x = 0; x < s.x; x++)
        {
            if (g[x][y].on)
                printf("#");
            else
                printf(".");
        }
        printf("\n");
    }
    printf("\n");
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

    std::ifstream fd(input);

    std::vector<Coord> coords;
    std::vector<Fold> folds;
    Coord size;

    parseInput(fd, coords, folds, size);

    Node **grid;
    grid = new Node *[size.x];
    for (int x = 0; x < size.x; x++)
    {
        grid[x] = new Node[size.y ];
    }

    for (auto c : coords)
    {
        grid[c.x][c.y].on = true;
    }

    printGrid(size, grid);

    for (auto f : folds)
    {
        int count = fold(f, size, grid);
        printGrid(size, grid);
        printf("Count: %d\n", count);
    }

    return 0;
}
