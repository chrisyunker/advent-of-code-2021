#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>


struct Game
{
    int idx1;
    int score1 = 0;
    int idx2;
    int score2 = 0;
};

struct Wins
{
    uint64_t p1 = 0ULL;
    uint64_t p2 = 0ULL;
};

int endScore = 21;

Wins DoTurn(Game game, int player, int roll, uint64_t count, bool calc)
{
    //printf("DoTurn: p1: %d, p2: %d, roll: %d\n", game.score1, game.score2, roll);

    if (calc)
    {
        Wins w1 = DoTurn(game, player, 3, 1 * count, false);
        Wins w2 = DoTurn(game, player, 4, 3 * count, false);
        Wins w3 = DoTurn(game, player, 5, 6 * count, false);
        Wins w4 = DoTurn(game, player, 6, 7 * count, false);
        Wins w5 = DoTurn(game, player, 7, 6 * count, false);
        Wins w6 = DoTurn(game, player, 8, 3 * count, false);
        Wins w7 = DoTurn(game, player, 9, 1 * count, false);

        return Wins{(w1.p1 + w2.p1 + w3.p1 + w4.p1 + w5.p1 + w6.p1 + w7.p1),
                    (w1.p2 + w2.p2 + w3.p2 + w4.p2 + w5.p2 + w6.p2 + w7.p2)};
    }
    else if (player == 1)
    {
        game.idx1 = (game.idx1 + roll) % 10;
        game.score1 += game.idx1 + 1;

        if (game.score1 >= endScore)
            return Wins{count, 0};
        else
        {
            return DoTurn(game, 2, 0, count, true);
        }
    }
    else
    {
        game.idx2 = (game.idx2 + roll) % 10;
        game.score2 += game.idx2 + 1;

        if (game.score2 >= endScore)
            return Wins{0, count};
        else
        {
            return DoTurn(game, 1, 0, count, true);
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

    Game game;

    std::ifstream fd(input);
    std::string line;

    std::getline(fd, line);
    game.idx1 = std::stoi(line.substr(28, 1)) - 1;
    printf("player1: %d\n", game.idx1+1);

    std::getline(fd, line);
    game.idx2 = std::stoi(line.substr(28, 1)) - 1;
    printf("player2: %d\n", game.idx2+1);

    Wins wins = DoTurn(game, 1, 0, 1, true);

    printf("Player 1 wins: %llu\n", wins.p1);
    printf("Player 2 wins: %llu\n", wins.p2);

    return 0;
}