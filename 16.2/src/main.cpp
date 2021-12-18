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

void printBits(std::string name, const Bits &bits)
{
    printf("%s: ", name.c_str());
    for (const bool b : bits)
    {
        printf("%d", b);
    }
    printf("\n");
}

uint64_t bitsToInt(const Bits &bits, uint16_t &ptr, uint16_t size)
{
    uint64_t sum = 0ULL;
    uint16_t last = ptr + size;
    while (ptr < last)
    {
        sum <<= 1;
        if (bits[ptr++])
            sum |= 1ULL;
    }
    return sum;
}

template <typename... Args>
void Log(int depth, const char *format, Args... args)
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

uint64_t parse(int depth, const Bits &bits, uint16_t &ptr)
{
    uint64_t sum = 0ULL;

    bitsToInt(bits, ptr, 3);
    //Log(depth, "VERSION: %d", vsn);

    uint16_t type = bitsToInt(bits, ptr, 3);
    Log(depth, "[%d] PARSE-----", type);

    if (type == 4)
    {
        Bits literal;
        bool lastNibble;
        do
        {
            lastNibble = bits[ptr++];
            uint16_t ptrlast = ptr + 4;
            while (ptr < ptrlast)
            {
                literal.push_back(bits[ptr++]);
            }
        } while (lastNibble);
        uint16_t lp = 0;
        sum = bitsToInt(literal, lp, literal.size());
        Log(depth, "LITERAL: %llu", sum);
    }
    else
    {
        std::vector<uint64_t> sums;
        bool lengthTypeId = bits[ptr++];

        if (lengthTypeId)
        {
            uint16_t subpackets = bitsToInt(bits, ptr, 11);
            Log(depth, "OPERATOR SUBPACKETS: %d", subpackets);
            while (subpackets > 0)
            {
                sums.push_back(parse(depth + 1, bits, ptr));
                subpackets--;
            }
        }
        else
        {
            uint16_t len = bitsToInt(bits, ptr, 15);
            Log(depth, "OPERATOR LENGTH: %d", len);

            uint16_t sublast = ptr + len;
            while (ptr < sublast)
            {
                sums.push_back(parse(depth + 1, bits, ptr));
            }
        }

        switch (type)
        {
        case 0:
            // sum
            for (auto s : sums)
            {
                sum += s;
            }
            break;
        case 1:
            // product
            for (int i = 0; i < sums.size(); i++)
            {
                if (i == 0)
                    sum = sums[i];
                else
                    sum *= sums[i];
            }
            break;
        case 2:
        {
            // min
            uint64_t min = sums[0];
            for (auto s : sums)
            {
                if (s < min)
                    min = s;
            }
            sum = min;
            break;
        }
        case 3:
        {
            // max
            uint64_t max = sums[0];
            for (auto s : sums)
            {
                if (s > max)
                    max = s;
            }
            sum = max;
            break;
        }
        case 5:
            // greater than
            //Log(depth, "%llu > %llu, SIZE: %d", sums[0], sums[1], sums.size());
            if (sums[0] > sums[1])
            {
                sum = 1;
            }
            break;
        case 6:
            // less than
            //Log(depth, "%llu < %llu, SIZE: %d", sums[0], sums[1], sums.size());
            if (sums[0] < sums[1])
            {
                sum = 1;
            }
            break;
        case 7:
            // equal
            //Log(depth, "%llu == %llu, SIZE: %d", sums[0], sums[1], sums.size());
            if (sums[0] == sums[1])
            {
                sum = 1;
            }
            break;
        default:
            printf("Unsupported type: %d\n", type);
            break;
        }
    }

    Log(depth, "[%d] END PARSE--- sum: %llu", type, sum);
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

    uint16_t ptr = 0;
    uint64_t sum = parse(0, transmission, ptr);

    printf("Sum of versions: %llu\n", sum);

    Bits extra;
    while (ptr < transmission.size())
    {
        extra.push_back(transmission[ptr++]);
    }
    printBits("Extra", extra);

    return 0;
}