#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

struct Coord
{
    int x;
    int y;
};

bool printBoard(Coord max, int **board)
{
    bool sync = true;
    for (int y = 0; y < max.y; y++)
    {
        for (int x = 0; x < max.x; x++)
        {
            printf("%d", board[y][x]);
            if (board[y][x] > 0)
                sync = false;
        }
        printf("\n");
    }
    printf("\n");
    return sync;
}

int flashBoard(Coord c, Coord max, int** board)
{
    int count = 1;

    std::vector<Coord> flashes;
    // x-1, y-1
    if (c.x > 0 && c.y > 0 && board[c.y-1][c.x-1] < 10)
    {
        board[c.y-1][c.x-1]++;
        if (board[c.y-1][c.x-1] == 10)
            flashes.push_back(Coord{c.x-1, c.y-1});
    }
    // x-1, y
    if (c.x > 0 && board[c.y][c.x-1] < 10)
    {
        board[c.y][c.x-1]++;
        if (board[c.y][c.x-1] == 10)
            flashes.push_back(Coord{c.x-1, c.y});
    }
    // x-1, y+1
    if (c.x > 0 && c.y < (max.y - 1) && board[c.y+1][c.x-1] < 10)
    {
        board[c.y+1][c.x-1]++;
        if (board[c.y+1][c.x-1] == 10)
            flashes.push_back(Coord{c.x-1, c.y+1});
    }

    // x, y-1
    if (c.y > 0 && board[c.y-1][c.x] < 10)
    {
        board[c.y-1][c.x]++;
        if (board[c.y-1][c.x] == 10)
            flashes.push_back(Coord{c.x, c.y-1});
    }
    // x, y+1
    if (c.y < (max.y - 1) && board[c.y+1][c.x] < 10)
    {
        board[c.y+1][c.x]++;
        if (board[c.y+1][c.x] == 10)
            flashes.push_back(Coord{c.x, c.y+1});
    }

    // x+1, y-1
    if (c.x < (max.x - 1) && c.y > 0 && board[c.y-1][c.x+1] < 10)
    {
        board[c.y-1][c.x+1]++;
        if (board[c.y-1][c.x+1] == 10)
            flashes.push_back(Coord{c.x+1, c.y-1});
    }
    // x+1, y
    if (c.x < (max.x - 1) && board[c.y][c.x+1] < 10)
    {
        board[c.y][c.x+1]++;
        if (board[c.y][c.x+1] == 10)
            flashes.push_back(Coord{c.x+1, c.y});
    }
    // x+1, y+1
    if (c.x < (max.x - 1) && c.y < (max.y - 1) && board[c.y+1][c.x+1] < 10)
    {
        board[c.y+1][c.x+1]++;
        if (board[c.y+1][c.x+1] == 10)
            flashes.push_back(Coord{c.x+1, c.y+1});
    }

    for (Coord c : flashes)
    {
        count += flashBoard(c, max, board);
    }

    return count;
}


int main(int argc, char *argv[])
{
    const int numArgs = 2;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    std::string input(argv[optind]);
    int steps = std::stoi(argv[optind + 1]);

    printf("Input file: %s\n", input.c_str());
    printf("Steps: %d\n\n", steps);

    // First count number of lines
    std::ifstream fd(input);
    int max_y = std::count(std::istreambuf_iterator<char>(fd),
                           std::istreambuf_iterator<char>(), '\n');
    fd.close();

    int max_x = 0;
    int **board;
    board = new int *[max_y];

    fd.open(input);
    std::string line;
    for(int y = 0; std::getline(fd, line); y++)
    {
        max_x = line.size();
        board[y] = new int[max_x];

        for(int x = 0; x < line.size(); x++)
        {
            int v = std::stoi(line.substr(x, 1));
            board[y][x] = v;
        }
    }

    Coord max{max_x, max_y};
    printBoard(max, board);

    int count = 0;
    int stepcount = 0;

    while (steps > 0)
    {
        std::vector<Coord> flashes;
        for (int y = 0; y < max_y; y++)
        {
            for (int x = 0; x < max_x; x++)
            {
                board[y][x]++;
                if (board[y][x] > 9)
                {
                    flashes.push_back(Coord{x,y});
                }
            }
        }

        for (Coord c : flashes)
        {
            count += flashBoard(c, max, board);
        }

        for (int y = 0; y < max_y; y++)
        {
            for (int x = 0; x < max_x; x++)
            {
                if (board[y][x] > 9)
                {
                    board[y][x] = 0;
                }
            }
        }
        stepcount++;

        printf("After step: %d\n", stepcount);
        bool sync = printBoard(max, board);

        if (sync)
            break;

        steps--;
    }

    printf("Flashes: %d\n", count);

    return 0;
}