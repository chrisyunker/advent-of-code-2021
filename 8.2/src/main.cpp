#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <bitset>


typedef std::bitset<7> digit;

struct DigitComparer
{
    bool operator() (const digit &b1, const digit &b2) const
    {
        return b1.to_ulong() < b2.to_ulong();
    }
};

struct Entry
{
    digit one;
    digit three;
    digit four;
    digit six;
    digit seven;
    digit eight;
    
    std::vector<digit> length5;
    std::vector<digit> length6;
    
    std::vector<digit> outputs;

    std::map<digit, int, DigitComparer> decode;
};

// abcdefg
// 6543210

digit stringToDigit(const std::string& s)
{
    digit d {};
    for (const char c : s)
    {
        switch (c)
        {
        case 'a':
            d |= 0b1000000;
            break;
        case 'b':
            d |= 0b0100000;
            break;
        case 'c':
            d |= 0b0010000;
            break;
        case 'd':
            d |= 0b0001000;
            break;
        case 'e':
            d |= 0b0000100;
            break;
        case 'f':
            d |= 0b0000010;
            break;
        case 'g':
            d |= 0b0000001;
            break;
        default:
            break;
        }
    }
    return d;
}

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
                digit r = stringToDigit(val);
                entry.outputs.push_back(r);
            }
            else
            {
                digit r = stringToDigit(val);

                switch (val.size())
                {
                case 2:
                    entry.one = r;
                    entry.decode[r] = 1;
                    break;
                case 4:
                    entry.four = r;
                    entry.decode[r] = 4;
                    break;
                case 3:
                    entry.seven = r;
                    entry.decode[r] = 7;
                    break;
                case 7:
                    entry.eight = r;
                    entry.decode[r] = 8;
                    break;
                case 5:
                    entry.length5.push_back(r);
                    break;
                case 6:
                    entry.length6.push_back(r);
                    break;
                default:
                    break;
                }
            }
        }
        prev = pos + 1;
    }
    if (prev < line.length())
    {
        std::string val = line.substr(prev, std::string::npos);
        digit r = stringToDigit(val);
        entry.outputs.push_back(r);
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

    int total = 0;
    for (Entry &entry : entries)
    {
        // a segment
        digit a_dig = entry.seven & ~entry.one;

        // 0, 6, 9
        for (const digit d : entry.length6)
        {
            if ((entry.one & d) != entry.one)
            {
                entry.six = d;
                entry.decode[d] = 6;
            }
        }

        // 2, 3, 5
        for (const digit d : entry.length5)
        {
            if ((entry.seven & d) == entry.seven)
            {
                entry.three = d;
                entry.decode[d] = 3;
            }
        }

        // c segment
        digit c_dig = entry.eight & ~entry.six;

        // f segment
        digit f_dig = entry.seven & ~a_dig & ~c_dig;

        // g segment
        digit g_dig = entry.three & ~entry.four & ~a_dig;

        // d segment
        digit d_dig = entry.three & ~a_dig & ~c_dig & ~f_dig & ~g_dig;

        // b segment
        digit b_dig = entry.four & ~d_dig & ~c_dig & ~f_dig;

        // e segment
        digit e_dig = entry.eight & ~a_dig & ~b_dig & ~c_dig & ~d_dig & ~f_dig & ~g_dig;

        digit n_0 = a_dig | b_dig | c_dig | e_dig | f_dig | g_dig;
        digit n_2 = a_dig | c_dig | d_dig | e_dig | g_dig;
        digit n_5 = a_dig | b_dig | d_dig | f_dig | g_dig;
        digit n_9 = a_dig | b_dig | c_dig | d_dig | f_dig | g_dig;

        entry.decode[n_0] = 0;
        entry.decode[n_2] = 2;
        entry.decode[n_5] = 5;
        entry.decode[n_9] = 9;

        int sum = 0;
        for (int i = 0; i < 4; i++)
        {
            int v = entry.decode[entry.outputs[i]];
            int p = 1;
            for (int j = i; j > 0; j--)
                p *= 10;
            sum += v * (1000 / p);
        }

        total += sum;
    }

    printf("Total: %d\n", total);

    return 0;
}
