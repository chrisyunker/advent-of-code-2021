#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

const int SIZE = 5;

struct coord
{
    int x;
    int y;
};

std::vector<int> parseLine(const std::string& line, const char del)
{
    std::vector<int> numbers;

    size_t start;
    size_t end = 0;
    while ((start = line.find_first_not_of(del, end)) != std::string::npos)
    {
        end = line.find(del, start);
        numbers.push_back(std::stoi(line.substr(start, end - start)));
    }

    return numbers;
};

class board
{
public:

    bool create(std::ifstream &fd);
    bool add(int v);
    int unmarked();
    void print();

private:
    bool bingo(int x, int y);

    bool** values;
    std::map<int, coord> vm;
};

int board::unmarked()
{
    int sum = 0;
    for (auto it = vm.begin(); it != vm.end(); it++)
    {
        int value = it->first;
        coord c = it->second;
        if (!values[c.x][c.y])
        {
            //printf("unmarked: [%d][%d] - %d\n", c.x, c.y, value);
            sum += value;
        }
        else
        {
            //printf("marked: [%d][%d] - %d\n", c.x, c.y, value);
        }
    }
    return sum;
}

bool board::bingo(int x, int y)
{
    // check x
    int bingo = true;
    for (int i = 0; i < SIZE; i++)
    {
        if (!values[x][i])
        {
            bingo = false;
            break;
        }
    }
    if (bingo) return true;

    // check y
    bingo = true;
    for (int i = 0; i < SIZE; i++)
    {
        if (!values[i][y])
        {
            bingo = false;
            break;
        }
    }
    if (bingo) return true;

    return false;
}

bool board::add(int v)
{
    bool result = false;

    if (vm.contains(v))
    {
        coord c = vm[v];
        values[c.x][c.y] = true;
        return bingo(c.x, c.y);
    }

    return result;
}

bool board::create(std::ifstream& fd)
{
    std::string line;
    values = new bool *[SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        if (!std::getline(fd, line))
        {
            //printf("Read an EOF\n");
            return false;
        }
        while (line.compare(std::string("")) == 0)
        {
            //printf("Read an empty line\n");
            std::getline(fd, line);
        }
        std::vector<int> row = parseLine(line, ' ');

        values[i] = new bool[SIZE];

        for (int j = 0; j < row.size(); j++)
        {
            values[i][j] = false;
            vm[row[j]] = coord{.x = i, .y = j};
        }
    }

    return true;
}

void board::print()
{
    printf("Print board\n");

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("[%d]", values[i][j]);
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

    std::vector<int> numbers;
    std::vector<board> boards;

    std::ifstream fd(input);
    std::string line;
    std::getline(fd, line);

    numbers = parseLine(line, ',');

    printf("Number of numbers: %lu\n", numbers.size());

    while (true)
    {
        board b = board();
        if (!b.create(fd)) break;
        boards.push_back(b);
        //b.print();
    }

    printf("Number of boards: %lu\n", boards.size());


    for (int n : numbers)
    {
        for (board b : boards)
        {
            if (b.add(n))
            {
                printf("BINGO: %d\n", n);
                b.print();

                int unmarked = b.unmarked();
                printf("Unmarked: %d\n", unmarked);
                printf("answer: %d\n", n * unmarked);

                return 0;
            }
        }
    }

    for (board b : boards)
    {
        b.print();
    }

    return 0;
}
