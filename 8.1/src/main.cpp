#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>


struct Entry
{
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
};

Entry parseInput(std::string& line)
{
    Entry entry;

    const std::string del = "abcdefg|";

    size_t pos;
    size_t prev = 0;
    bool divider = false;
    while ((pos = line.find_first_not_of(del, prev)) != std::string::npos)
    {
        if (pos > prev)
        {
            std::string val = line.substr(prev, pos - prev);
            if (val == "|")
            {
                divider = true;
            }
            else if (divider)
            {
                entry.outputs.push_back(val);
            }
            else
            {
                entry.inputs.push_back(val);
            }
        }
        prev = pos + 1;
    }
    if (prev < line.length())
    {
        std::string val = line.substr(prev, std::string::npos);
        entry.outputs.push_back(val);
    }

    return entry;
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

    std::ifstream fd(input);

    std::string line;
    std::vector<Entry> entries;
    while (std::getline(fd, line))
    {
        entries.push_back(parseInput(line));
    }
    printf("Number of entries: %lu\n", entries.size());

    int easy_digits = 0;
    for (const Entry &entry : entries)
    {
        for (const std::string &output : entry.outputs)
        {
            switch (output.size())
            {
            case 2:
                easy_digits++;
                break;
            case 4:
                easy_digits++;
                break;
            case 3:
                easy_digits++;
                break;
            case 7:
                easy_digits++;
                break;
            default:
                break;
            }
        }
    }

    printf("Total number of 1,4,7,8 digits: %d\n", easy_digits);

    return 0;
}
