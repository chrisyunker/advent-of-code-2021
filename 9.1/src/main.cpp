#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>


void printBoard(int sx, int sy, int **board)
{
    for (int y = 0; y < sy; y++)
    {
        for (int x = 0; x < sx; x++)
        {
            printf("[%d]", board[y][x]);
        }
        printf("\n");
    }
    printf("\n");
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

    int** board;

    // First count number of lines
    std::ifstream fd(input);
    int y_size = std::count(std::istreambuf_iterator<char>(fd), 
            std::istreambuf_iterator<char>(), '\n');
    fd.close();

    board = new int*[y_size];

    fd.open(input);
    std::string line;
    int x_size = 0;
    int y = 0;
    while (std::getline(fd, line))
    {
        board[y] = new int[line.length()];
        x_size = line.length();

        for (int x = 0; x < line.length(); x++)
        {
            board[y][x] = std::stoi(line.substr(x, 1));
        }

        y++;
    }
    printf("Board size: x:%d y:%d\n", x_size, y_size);

    printBoard(x_size, y_size, board);

    std::vector<int> lows;
    for (int y = 0; y < y_size; y++)
    {
        for (int x = 0; x < x_size; x++)
        {
            //printf("[%d][%d] : %d\n", x, y, board[y][x]);
            if (x > 0 && (board[y][x] >= board[y][x-1]))
                continue;
            if (x < (x_size - 1) && (board[y][x] >= board[y][x+1]))
                continue;
            if (y > 0 && (board[y][x] >= board[y-1][x]))
                continue;
            if (y < (y_size - 1) && (board[y][x] >= board[y+1][x]))
                continue;
            lows.push_back(board[y][x]);                
        }
    }
    
    int risk = 0;
    for (auto l : lows)
    {
        //printf("Low spot value: %d\n", l);
        risk += l + 1;
    }

    printf("Total risk: %d\n", risk);

    return 0;
}
