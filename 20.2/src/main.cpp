#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

using Image = std::vector<std::vector<int>>;

int enhance[512];
int buffer = 10;

void parseImageAlg(std::string line)
{
    for (int i = 0; i < 512; i++)
    {
        if (line[i] == '.')
            enhance[i] = 0;
        else if (line[i] == '#')
            enhance[i] = 1;
        else
            printf("ERROR: Bad enhancement algorithm input\n");
    }
}

void printImageAlg()
{
    for (int i = 0; i < 512; i++)
    {
        if (enhance[i] == 0)
            printf(".");
        else if (enhance[i] == 1)
            printf("#");
        else
            printf("E");
    }
    printf("\n");
}

Image parseImage(std::ifstream& fd)
{
    Image image;

    std::string line;
    int width = 0;
    while (std::getline(fd, line))
    {
        if (line == "")
            continue;

        width = line.size();

        // Add row buffer
        if (image.size() == 0)
        {
            for (int i=0; i < buffer; i++)
            {
                std::vector<int> br;
                for (int j=0; j < width + (2*buffer); j++)
                    br.push_back(0);
                image.push_back(br);
            }
        }

        std::vector<int> row;

        // Add column buffer
        for (int j = 0; j < buffer; j++)
            row.push_back(0);

        for (auto c : line)
        {
            if (c == '.')
                row.push_back(0);
            else if (c == '#')
                row.push_back(1);
            else
                printf("ERROR: Bad image input\n");
        }

        // Add column buffer
        for (int j = 0; j < buffer; j++)
            row.push_back(0);

        image.push_back(row);
    }

    // Add row buffer
    for (int i = 0; i < buffer; i++)
    {
        std::vector<int> br;
        for (int j = 0; j < width + (2 * buffer); j++)
            br.push_back(0);
        image.push_back(br);
    }

    return image;
}

void printImage(const Image& image)
{
    int light = 0;

    for (auto r : image)
    {
        for (auto p : r)
        {
            if (p == 0)
                printf(".");
            else if (p == 1)
            {
                light++;
                printf("#");
            }
            else
                printf("E");
        }
        printf("\n");
    }
    printf("Light pixels: %d\n", light);
}

void initImage(int rows, int cols, Image &image)
{
    for (int i = 0; i < rows; i++)
    {
        std::vector<int> row;
        for (int j = 0; j < cols; j++)
        {
            row.push_back(0);
        }
        image.push_back(row);
    }
}

int getEnhance(int x, int y, const Image &image)
{
    int maxX = image[0].size();
    int maxY = image.size();

    std::vector<int> bits;

    int infBit = 0;
    if (image[y][x] == 1)
    {
        infBit = 1;
    }
    else
    {
        infBit = 0;
    }

    if (y == 0 || x == 0)
        bits.push_back(infBit);
    else
        bits.push_back(image[y - 1][x - 1]);

    if (y == 0)
        bits.push_back(infBit);
    else
        bits.push_back(image[y - 1][x]);

    if (y == 0 || x == maxX - 1)
        bits.push_back(infBit);
    else
        bits.push_back(image[y - 1][x + 1]);

    if (x == 0)
        bits.push_back(infBit);
    else
        bits.push_back(image[y][x - 1]);

    bits.push_back(image[y][x]);

    if (x == maxX - 1)
        bits.push_back(infBit);
    else
        bits.push_back(image[y][x + 1]);

    if (y == maxY - 1 || x == 0)
        bits.push_back(0);
    else
        bits.push_back(image[y + 1][x - 1]);

    if (y == maxY - 1)
        bits.push_back(infBit);
    else
        bits.push_back(image[y + 1][x]);

    if (y == maxY - 1 || x == maxX - 1)
        bits.push_back(infBit);
    else
        bits.push_back(image[y + 1][x + 1]);

    uint16_t sum = 0UL;
    for (int i = 0; i < 9; i++)
    {
        sum <<= 1;
        if (bits[i] == 1)
            sum |= 1UL;
    }

    return enhance[sum];
}

Image enhanceImage(const Image& image)
{
    Image image2;
    initImage(image.size(), image[0].size(), image2);

    for (int y = 0; y < image.size(); y++)
    {
        for (int x = 0; x < image[0].size(); x++)
        {
            {
                image2[y][x] = getEnhance(x, y, image);
            }
        }
    }

    return image2;
}

int main(int argc, char *argv[])
{
    const int numArgs = 2;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    std::string input(argv[optind]);
    int b = std::stoi(argv[optind+1]);

    printf("Input file: %s\n", input.c_str());
    printf("Buffer: %d\n", b);
    buffer = b;

    int enhancements = 50;

    std::ifstream fd(input);
    std::string line;
    std::getline(fd, line);

    parseImageAlg(line);
    //printImageAlg();

    Image image = parseImage(fd);
    printImage(image);

    for (int i=0; i < enhancements; i++)
    {
        image =  enhanceImage(image);
        printImage(image);
    }

    return 0;
}