#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

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

    //std::vector<std::string> goodLines;
    std::vector<long> scores;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        std::vector<char> stack;
        bool corrupt = false;
        for (auto c : line)
        {
            switch (c)
            {
            case '[':
                stack.push_back(c);
                break;
            case '{':
                stack.push_back(c);
                break;
            case '<':
                stack.push_back(c);
                break;
            case '(':
                stack.push_back(c);
                break;
            case ']':
                if (stack.back() == '[')
                    stack.pop_back();
                else
                {
                    corrupt = true;
                }
                break;
            case '}':
                if (stack.back() == '{')
                    stack.pop_back();
                else
                {
                    corrupt = true;
                }
                break;
            case '>':
                if (stack.back() == '<')
                    stack.pop_back();
                else
                {
                    corrupt = true;
                }
                break;
            case ')':
                if (stack.back() == '(')
                    stack.pop_back();
                else
                {
                    corrupt = true;
                }
                break;
            default:
                break;
            }
            if (corrupt)
                break;
        }
        
        if (corrupt)
            continue;
        printf("Incomplete line: %s, stack:%lu\n", line.c_str(), stack.size());

        long score = 0;
        while (stack.size() > 0)
        {
            char c = stack.back();
            stack.pop_back();
            switch (c)
            {
            case '[':
                score = (score * 5) + 2;
                break;
            case '{':
                score = (score * 5) + 3;
                break;
            case '<':
                score = (score * 5) + 4;
                break;
            case '(':
                score = (score * 5) + 1;
                break;
            default:
                break;
            }
        }
        scores.push_back(score);
        printf("Line score: %ld\n", score);
    }

    std::sort(scores.begin(), scores.end());
    long middle = scores[scores.size() / 2];

    printf("Middle score: %ld\n", middle);

    return 0;
}