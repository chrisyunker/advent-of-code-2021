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

    std::vector<std::string> goodLines;
    int score = 0;

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
                    score += 57;
                    corrupt = true;
                }
                break;
            case '}':
                if (stack.back() == '{')
                    stack.pop_back();
                else
                {
                    score += 1197;
                    corrupt = true;
                }
                break;
            case '>':
                if (stack.back() == '<')
                    stack.pop_back();
                else
                {
                    score += 25137;
                    corrupt = true;
                }
                break;
            case ')':
                if (stack.back() == '(')
                    stack.pop_back();
                else
                {
                    score += 3;
                    corrupt = true;
                }
                break;
            default:
                break;
            }
            if (corrupt)
                break;
        }
        if (!corrupt)
            goodLines.push_back(line);
        else
            printf("Corrupt line: %s\n", line.c_str());
    }

    printf("Score: %d\n", score);

    return 0;
}