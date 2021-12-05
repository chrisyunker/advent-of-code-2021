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

struct spot
{
    int value;
    bool checked;
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

class Board
{
public:
    bool Create(int count, std::ifstream &fd);
    bool Add(int v);
    int SumUnmarked();
    void Print();

    int id = 0;
    bool done = false;

private:
    bool CheckBingo(int x, int y);

    spot** values;
    std::map<int, coord> vm;
};

int Board::SumUnmarked()
{
    int sum = 0;
    for (auto it = vm.begin(); it != vm.end(); it++)
    {
        int value = it->first;
        coord c = it->second;
        if (!values[c.x][c.y].checked)
        {
            sum += value;
        }
    }
    return sum;
}

bool Board::CheckBingo(int x, int y)
{
    // check x
    int bingo = true;
    for (int i = 0; i < SIZE; i++)
    {
        if (!values[x][i].checked)
        {
            bingo = false;
            break;
        }
    }
    if (bingo)
    {
        done = true;
        return true;
    }

    // check y
    bingo = true;
    for (int i = 0; i < SIZE; i++)
    {
        if (!values[i][y].checked)
        {
            bingo = false;
            break;
        }
    }
    if (bingo)
    {
        done = true;
        return true;
    }

    return false;
}

bool Board::Add(int v)
{
    if (done)
    {
        return false;
    }

    if (vm.contains(v))
    {
        coord c = vm[v];
        
        values[c.x][c.y].checked = true;
        bool result = this->CheckBingo(c.x, c.y);
        return result;
    }

    return false;
}

bool Board::Create(int count, std::ifstream& fd)
{
    id = count;
    std::string line;
    values = new spot *[SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        if (!std::getline(fd, line))
        {
            return false;
        }
        while (line.compare(std::string("")) == 0)
        {
            std::getline(fd, line);
        }
        std::vector<int> row = parseLine(line, ' ');

        values[i] = new spot[SIZE];

        for (int j = 0; j < SIZE; j++)
        {
            values[i][j] = spot{.checked = false, .value = row[j]};
            vm[row[j]] = coord{.x = i, .y = j};
        }
    }

    return true;
}

void Board::Print()
{
    printf("Print Board: %d\n", id);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("[%d, %d]", values[i][j].value, values[i][j].checked);
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

    std::vector<int> numbers;
    std::vector<Board*> boards;

    std::ifstream fd(input);
    std::string line;
    std::getline(fd, line);

    numbers = parseLine(line, ',');

    printf("Number of called numbers: %lu\n", numbers.size());

    int count = 0;
    while (true)
    {
        Board* board = new Board();
        if (!board->Create(count, fd)) break;
        boards.push_back(board);
        count++;
    }

    int numBoards = boards.size();
    printf("Number of boards: %d\n", numBoards);

    // Cycle through called numbers
    for (int n : numbers)
    {
        for (Board* board : boards)
        {
            if (board->Add(n))
            {
                numBoards -= 1;
                if (numBoards == 0)
                {
                    board->Print();

                    int unmarked = board->SumUnmarked();
                    printf("Unmarked: %d\n", unmarked);
                    printf("answer: %d\n", n * unmarked);

                    return 0;
                }
            }
        }
    }

    return 0;
}
