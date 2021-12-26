#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

int die = 1;
int rolls = 0;

struct Player
{
    int idx = 0;
    int score = 0;
};

int RollDice()
{
    int result = 0;
    for (int i=0; i < 3; i++)
    {
        result += die;
        if (die == 100)
            die = 1;
        else
            die++;
        rolls++;
    }
    return result;
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

    Player p1, p2;

    std::ifstream fd(input);
    std::string line;

    std::getline(fd, line);
    p1.idx = std::stoi(line.substr(28, 1)) - 1;
    printf("player1: %d\n", p1.idx+1);

    std::getline(fd, line);
    p2.idx = std::stoi(line.substr(28, 1)) - 1;
    printf("player2: %d\n", p2.idx+1);

    while (true)
    {
        int roll;
        
        roll = RollDice();
        p1.idx = (p1.idx + roll) % 10;
        p1.score += (p1.idx + 1);

        if (p1.score >= 1000)
        {
            printf("P1: %d, P2: %d\n", p1.score, p2.score);

            printf("Die rolls: %d\n", rolls);
            printf("Losing player: %d\n", p2.score);
            printf("Value: %d\n", rolls * p2.score);
            break;
        }

        roll = RollDice();
        p2.idx = (p2.idx + roll) % 10;
        p2.score += (p2.idx + 1);

        if (p2.score >= 1000)
        {
            printf("P1: %d, P2: %d\n", p1.score, p2.score);

            printf("Die rolls: %d\n", rolls);
            printf("Losing player: %d\n", p1.score);
            printf("Value: %d\n", rolls * p1.score);
            break;
        }

    }

    return 0;
}