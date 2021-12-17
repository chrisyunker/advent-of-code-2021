#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <queue>

struct Node
{
    int risk;
    int sum;
};

struct Coord
{
    int x;
    int y;
};

Node **board;
Coord max;
std::queue<Coord> coords;

void printBoard()
{
    for (int y = 0; y <= max.y; y++)
    {
        for (int x = 0; x <= max.x; x++)
        {
            printf("%d", board[y][x].risk);
        }
        printf("\n");
    }
    printf("\n");
}

void traverse(int x, int y)
{
    if (x > 0)
    {
        if (board[y][x-1].sum == 0)
        {
            board[y][x - 1].sum = board[y][x - 1].risk + board[y][x].sum;
            coords.push(Coord{x-1, y});
        }
        else if ((board[y][x-1].risk + board[y][x].sum) < board[y][x-1].sum)
        {
            board[y][x-1].sum = board[y][x-1].risk + board[y][x].sum;
            coords.push(Coord{x-1, y});
        }
    }
    if (x < max.x)
    {
        if (board[y][x+1].sum == 0)
        {
            board[y][x + 1].sum = board[y][x + 1].risk + board[y][x].sum;
            coords.push(Coord{x+1, y});
        }
        else if ((board[y][x+1].risk + board[y][x].sum) < board[y][x+1].sum)
        {
            board[y][x+1].sum = board[y][x+1].risk + board[y][x].sum;
            coords.push(Coord{x+1, y});
        }
    }
    if (y > 0)
    {
        if (board[y - 1][x].sum == 0)
        {
            board[y - 1][x].sum = board[y - 1][x].risk + board[y][x].sum;
            coords.push(Coord{x, y - 1});
        }
        else if ((board[y - 1][x].risk + board[y][x].sum) < board[y - 1][x].sum)
        {
            board[y - 1][x].sum = board[y - 1][x].risk + board[y][x].sum;
            coords.push(Coord{x, y - 1});
        }
    }
    if (y < max.y)
    {
        if (board[y+1][x].sum == 0)
        {
            board[y+1][x].sum = board[y+1][x].risk + board[y][x].sum;
            coords.push(Coord{x, y+1});
        }
        else if ((board[y+1][x].risk + board[y][x].sum) < board[y+1][x].sum)
        {
            board[y+1][x].sum = board[y+1][x].risk + board[y][x].sum;
            coords.push(Coord{x, y+1});
        }
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

    // First count number of lines
    std::ifstream fd(input);
    int max_y = std::count(std::istreambuf_iterator<char>(fd),
                           std::istreambuf_iterator<char>(), '\n');
    fd.close();

    int max_x = 0;
    board = new Node *[max_y * 5];

    fd.open(input);
    std::string line;
    for(int y = 0; std::getline(fd, line); y++)
    {
        max_x = line.size();
        board[y] = new Node[max_x * 5];

        for(int x = 0; x < line.size(); x++)
        {
            int v = std::stoi(line.substr(x, 1));
            board[y][x].risk = v;
        }
    }
    for(int y = 0; y < max_y; y++)
    {
        for(int x = max_x; x < (max_x * 5); x++)
        {
            int newRisk = board[y][x - max_x].risk + 1;
            if (newRisk > 9)
                newRisk = 1;
            board[y][x].risk = newRisk;
        }
    }
    for(int y = max_y; y < (max_y * 5); y++)
    {
        board[y] = new Node[max_x * 5];

        for(int x = 0; x < (max_x * 5); x++)
        {
            int newRisk = board[y - max_y][x].risk + 1;
            if (newRisk > 9)
                newRisk = 1;
            board[y][x].risk = newRisk;
        }
    }
    max = Coord{ (max_x * 5) - 1, (max_y * 5) - 1};
        
    printBoard();

    coords.push(Coord{ 0, 0 });
    board[0][0].sum = 0;
    board[0][0].risk = 0;
    
    while (!coords.empty())
    {
        Coord c = coords.front();
        coords.pop();
        traverse(c.x, c.y);
    }

    printBoard();

    printf("Least risk %d\n", board[max.y][max.x].sum);

    return 0;
}