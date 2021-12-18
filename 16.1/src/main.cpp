#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <bitset>

typedef std::vector<bool> Bits;

Bits decode(char c)
{
    switch (c)
    {
    case '0':
        return Bits{false, false, false, false};
    case '1':
        return Bits{false, false, false, true};
    case '2':
        return Bits{false, false, true, false};
    case '3':
        return Bits{false, false, true, true};
    case '4':
        return Bits{false, true, false, false};
    case '5':
        return Bits{false, true, false, true};
    case '6':
        return Bits{false, true, true, false};
    case '7':
        return Bits{false, true, true, true};
    case '8':
        return Bits{true, false, false, false};
    case '9':
        return Bits{true, false, false, true};
    case 'A':
        return Bits{true, false, true, false};
    case 'B':
        return Bits{true, false, true, true};
    case 'C':
        return Bits{true, true, false, false};
    case 'D':
        return Bits{true, true, false, true};
    case 'E':
        return Bits{true, true, true, false};
    case 'F':
        return Bits{true, true, true, true};
    default:
        return Bits{false, false, false, false};
    }
}

uint32_t bitsToInt(const Bits& bits, int &ptr, int size)
{
    uint32_t sum = 0;
    int last = ptr + size;
    while (ptr < last)
    {
        sum <<= 1;
        if (bits[ptr++])
            sum |= 1;
    }
    return sum;
}

void printBits(std::string name, const Bits &bits)
{
    printf("%s: ", name.c_str());
    for (const bool b : bits)
    {
        printf("%d", b);
    }
    printf("\n");
}

template <typename... Args>
void Log(int depth, const char* format, Args... args)
{
    std::string msg;
    while (depth > 0)
    {
        msg += " ";
        depth--;
    }
    msg += format;
    msg += "\n";
    printf(msg.c_str(), args...);
}

int16_t parse(int depth, const Bits& bits, int& ptr, int last)
{

    Log(depth, "PARSE-----", 0);
    int sum = 0;

    int vsn = bitsToInt(bits, ptr, 3);
    Log(depth, "VERSION: %d", vsn);
    sum += vsn;

    int type = bitsToInt(bits, ptr, 3);

    if (type == 4)
    {
        Bits literal;
        bool lastNibble;
        do
        {
            lastNibble = bits[ptr++];
            int last = ptr + 4;
            while (ptr < last)
            {
                literal.push_back(bits[ptr++]);
            }
        } while (lastNibble);
        int lp = 0;
        int value = bitsToInt(literal, lp, literal.size());
        Log(depth, "LITERAL: %d", value);
    }
    else
    {
        bool lengthTypeId = bits[ptr++];
        Log(depth, "OPERATOR type: %d", lengthTypeId);

        if (lengthTypeId)
        {
            int subpackets = bitsToInt(bits, ptr, 11);
            Log(depth, "SUBPACKETS: %d", subpackets);
            while (subpackets > 0)
            {
                sum += parse(depth+1, bits, ptr, last);
                subpackets--;
            }
        }
        else
        {
            int len = bitsToInt(bits, ptr, 15);
            Log(depth, "Length: %d", len);

            int sublast = ptr+len;

            while ((sublast - ptr) > 3)
            {
                sum += parse(depth+1, bits, ptr, sublast);
            }
        }
    }
    Log(depth, "END PARSE-----", 0);
    return sum;
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

    std::ifstream fd(input);
    std::string line;
    std::getline(fd, line);

    printf("Input: %s\n", line.c_str());

    Bits transmission;
    for (auto c : line)
    {
        Bits n = decode(c);
        transmission.insert(transmission.end(), n.begin(), n.end());
    }
    
    int ptr = 0;
    int sum = parse(0, transmission, ptr, transmission.size());

    printf("Sum of versions: %d\n", sum);

    return 0;
}