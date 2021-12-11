#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

struct point
{
    int value = 0;
    bool seen = false;
};

struct coord
{
    int x;
    int y;
};

void printBoard(coord max, point **board)
{
    for (int y = 0; y < max.y; y++)
    {
        for (int x = 0; x < max.x; x++)
        {
            printf("[%d]", board[y][x].value);
        }
        printf("\n");
    }
    printf("\n");
}

int mapBasin(coord max, coord c, point **board)
{
    int size = 1;
    board[c.y][c.x].seen = true;

    if (c.x > 0 && (board[c.y][c.x-1].value < 9) && !board[c.y][c.x-1].seen)
        size += mapBasin(max, coord{ c.x-1, c.y }, board);
    if (c.x < (max.x - 1) && (board[c.y][c.x + 1].value < 9) && !board[c.y][c.x + 1].seen)
        size += mapBasin(max, coord{ c.x+1, c.y }, board);
    if (c.y > 0 && (board[c.y-1][c.x].value < 9) && !board[c.y-1][c.x].seen)
        size += mapBasin(max, coord{ c.x, c.y-1 }, board);
    if (c.y < (max.y - 1) && (board[c.y+1][c.x].value < 9) && !board[c.y+1][c.x].seen)
        size += mapBasin(max, coord{ c.x, c.y+1 }, board);
    
    return size;
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

    printf("Input file: %s\n", input.c_str());

    point** board;

    // First count number of lines
    std::ifstream fd(input);
    int y_size = std::count(std::istreambuf_iterator<char>(fd), 
            std::istreambuf_iterator<char>(), '\n');
    fd.close();

    board = new point*[y_size];

    fd.open(input);
    std::string line;
    int x_size = 0;
    int y = 0;
    while (std::getline(fd, line))
    {
        board[y] = new point[line.length()];
        x_size = line.length();

        for (int x = 0; x < line.length(); x++)
        {
            board[y][x].value = std::stoi(line.substr(x, 1));
        }

        y++;
    }
    printf("Board size: x:%d y:%d\n", x_size, y_size);

    printBoard(coord{ x_size, y_size }, board);

    std::vector<int> basins;
    for (int y = 0; y < y_size; y++)
    {
        for (int x = 0; x < x_size; x++)
        {
            if (board[y][x].value < 9 && !board[y][x].seen)
            {
                int size = mapBasin(coord{x_size, y_size}, coord{ x, y }, board);
                basins.push_back(size);
            }            
        }
    }

    std::sort(basins.begin(), basins.end()); 
    
    int total = 1;
    for (int i = (basins.size() - 1); i >= basins.size()-3; i--)
    {
        total *= basins[i];
    }

    printf("Total size: %d\n", total);

    return 0;
}