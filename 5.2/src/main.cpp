#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

void parseInput(std::ifstream &fd, std::vector<int*>& rows)
{
    std::string del = "->, \n";

    std::string line;
    while (std::getline(fd, line))
    {
        int *row = new int[4]; 
        int i = 0;
        size_t pos;
        size_t prev = 0;
        while ((pos = line.find_first_of(del, prev)) != std::string::npos)
        {
            if (pos > prev)
            {
                int num = std::stoi(line.substr(prev, pos - prev));
                row[i++] = num;
            }
            prev = pos + 1;
        }
        if (prev < line.length())
        {
            int num = std::stoi(line.substr(prev, std::string::npos));
            row[i++] = num;
        }
        rows.push_back(row);
    }
};

int count(int min, int max, int** board)
{
    int total = 0;
    for (int y = 0; y <= max; y++)
    {
        for (int x = 0; x <= max; x++)
        {
            if (board[x][y] >= min)
            {
                total += 1;
            }
        }
    }
    return total;
}

void print(int max, int** board)
{
    for (int y = 0; y <= max; y++)
    {
        for (int x = 0; x <= max; x++)
        {
            printf("[%d]", board[x][y]);
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

    printf("input file: %s\n", input.c_str());

    std::ifstream fd(input);
   
    std::vector<int*> rows;
    parseInput(fd, rows);

    printf("Number of numbers: %lu\n", rows.size());

    int max = 0;
    for (auto row : rows)
    {
        for (int i = 0; i < 4; i++)
        {
            if (row[i] > max)
                max = row[i];
        }
    }
    printf("max: %d\n", max);

    int **board = new int*[max];

    for (int x = 0; x <= max; x++)
    {
        board[x] = new int[max];
        for (int y = 0; y <= max; y++)
        {
            board[x][y] = 0;
        }
    }
    
    for (auto row : rows)
    {
        int x1 = row[0];
        int y1 = row[1];
        int x2 = row[2];
        int y2 = row[3];

        if (x1 == x2)
        {
            if (y2 > y1)
            {
                for (int i = y1; i <= y2; i++)
                {
                    board[x1][i] += 1;
                }
            }
            else
            {
                for (int i = y2; i <= y1; i++)
                {
                    board[x1][i] += 1;
                }
            }
        }
        else if (y1 == y2)
        {
            if (x2 > x1)
            {
                for (int i = x1; i <= x2; i++)
                {
                    board[i][y1] += 1;
                }
            }
            else
            {
                for (int i = x2; i <= x1; i++)
                {
                    //("incx: [%d][%d] val:%d\n", i, y1, board[i][y1]);
                    board[i][y1] += 1;
                }
            }
        }
        else
        {
            if (abs(x1 - x2) == abs(y1 - y2))
            {
                while (x1 != x2)
                {
                    board[x1][y1] += 1;

                    if (x1 > x2)
                        x1--;
                    else
                        x1++;

                    if (y1 > y2)
                        y1--;
                    else
                        y1++;
                }
                board[x2][y2] += 1;
            }
        }
    }

    print(max, board);

    int overlap = count(2, max, board);
    printf("Overlap: %d\n", overlap);

    return 0;
}
