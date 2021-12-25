#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <tuple>
#include <set>


using Pair = std::tuple<int, int>;

using Coord = std::tuple<int, int, int>;

using Coords =  std::vector<Coord>;

struct CoordSet
{
    Coords coords;
    std::map<Coord, Pair> dist;
};

std::set<Coord> beacons;

struct Scanner
{
    int number = -1;
    Coord delta;
    int tsi = 0;
    bool done = false;
    Coords coords;
    std::vector<CoordSet> sets;
};

Coord Diff(Coord a, Coord b)
{
    return Coord(std::get<0>(a) - std::get<0>(b),
                 std::get<1>(a) - std::get<1>(b),
                 std::get<2>(a) - std::get<2>(b));
}
Coord Add(Coord a, Coord b)
{
    return Coord(std::get<0>(a) + std::get<0>(b),
                 std::get<1>(a) + std::get<1>(b),
                 std::get<2>(a) + std::get<2>(b));
}

Coords ParseScanner(std::ifstream &fd)
{
    Coords coords;
    std::string line;
    while (std::getline(fd, line))
    {
        if (line == "")
            break;
        int pos, next;
        next = line.find_first_of(",", 0);
        int x = std::stoi(line.substr(0, next));
        pos = next+1;
        next = line.find_first_of(",", pos);
        int y = std::stoi(line.substr(pos, next));
        pos = next+1;
        next = line.find_first_of(",", pos);
        int z = std::stoi(line.substr(pos, next));
        //printf("Found: %d, %d, %d\n", x, y, z);
        coords.push_back(Coord{x, y, z});
    }
    return coords;
}

std::vector<Scanner> ParseInput(std::ifstream &fd)
{
    std::vector<Scanner> scanners;
    std::string line;
    int number = -1;
    while (std::getline(fd, line))
    {
        if (line == "")
            continue;
        if (number == -1)
        {
            int pos, pos2;
            if ((pos = line.find_first_of("r", 0)) != std::string::npos)
            {
                pos2 = line.find_first_of(" ", pos + 1);
                number = std::stoi(line.substr(pos + 1, pos2));
            }
            Scanner s;
            s.number = number;
            s.coords = ParseScanner(fd);
            //printf("ParseScanner: %d\n", number);
            scanners.push_back(s);
            number = -1;
        }
    }

    return scanners;
}

void printCoord(Coord c)
{
    printf("[%d,%d,%d]\n", std::get<0>(c), std::get<1>(c), std::get<2>(c));
}

void printScanner(Scanner s)
{
    printf("SCANNER START ----\n");
    for (auto &c : s.coords)
    {
        printCoord(c);
    }
    printf("SCANNER END ----\n");
}

void printScanners(std::vector<Scanner> scanners)
{
    for (auto& s : scanners)
    {
        printScanner(s);
    }
}

int trMap[24][3] = {{ 1,  2,  3},
                    {-2,  1,  3},
                    {-1, -2,  3},
                    { 2, -1,  3},
                    
                    {-1,  2, -3},
                    {-2, -1, -3},
                    { 1, -2, -3},
                    { 2,  1, -3},
                    
                    { 3,  2, -1},
                    {-2,  3, -1},
                    {-3, -2, -1},
                    { 2, -3, -1},
                    
                    {-3,  2,  1},
                    {-2, -3,  1},
                    { 3, -2,  1},
                    { 2,  3,  1},

                    {-3,  1, -2},
                    {-1, -3, -2},
                    { 3, -1, -2},
                    { 1,  3, -2},

                    { 3,  1,  2},
                    {-1,  3,  2},
                    {-3, -1,  2},
                    { 1, -3,  2}};

int Op(int m, Coord c)
{
    int x = std::get<0>(c);
    int y = std::get<1>(c);
    int z = std::get<2>(c);
    
    switch (m)
    {
    case 1:
        return x;
    case -1:
        return x * -1;
    case 2:
        return y;
    case -2:
        return y * -1;
    case 3:
        return z;
    case -3:
        return z * -1;
    }
    printf("Failed transpose\n");
    return 0;
}

void Transpose(Scanner& s)
{
    CoordSet cs;

    for (auto m : trMap)
    {
        CoordSet cs;
        for (const auto &c : s.coords)
        {
            int x = Op(m[0], c);
            int y = Op(m[1], c);
            int z = Op(m[2], c);
            Coord n(x, y, z);
            cs.coords.push_back(n);
        }

        // Calculate diffs between all points
        std::map<std::tuple<int, int>, int> unique;

        for (int i=0; i < cs.coords.size(); i++)
        {
            for (int j=0; j < cs.coords.size(); j++)
            {
                if (i == j)
                    continue;

                Coord diff = Diff(cs.coords[i], cs.coords[j]);

                cs.dist[diff] = Pair(i, j);
            }
        }
        s.sets.push_back(cs);
    }
}

bool CompareScanners(Scanner& a, Scanner& b)
{
    if (a.number == b.number)
        return false;
    if (!a.done)
        return false;
    if (a.done && b.done)
        return false;
    //printf("Compare: %d with %d\n", a.number, b.number);

    for (int i = 0; i < b.sets.size(); i++)
    {
        std::set<Coord> aMatch;
        std::set<Coord> bMatch;
        Coord delta;

        for (auto &[k, v] : b.sets[i].dist)
        {
            if (a.sets[a.tsi].dist.contains(k))
            {
                int a1 = std::get<0>(a.sets[a.tsi].dist[k]);
                int a2 = std::get<1>(a.sets[a.tsi].dist[k]);
                aMatch.insert(a.sets[a.tsi].coords[a1]);
                aMatch.insert(a.sets[a.tsi].coords[a2]);
                int b1 = std::get<0>(b.sets[i].dist[k]);

                delta = Diff(a.sets[a.tsi].coords[a1], b.sets[i].coords[b1]);
                //printf("Delta: [%d,%d,%d]\n", std::get<0>(delta), std::get<1>(delta), std::get<2>(delta));
            }
        }

        if (aMatch.size() < 12)
        {
            continue;
        }

        printf("%d - %d matched %lu beacons\n", a.number, b.number, aMatch.size());

        b.delta = Add(delta, a.delta);
        b.done = true;
        b.tsi = i;
        for (Coord c : b.sets[i].coords)
        {
            beacons.insert(Add(c, b.delta));
        }
        return true;
    }

    return false;
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
    std::vector<Scanner> scanners = ParseInput(fd);

    for (auto& s: scanners)
    {
        Transpose(s);
    }
    //printScanners(scanners);
    printf("Total Scanners: %lu\n", scanners.size());

    int initScanner = 0;
    scanners[initScanner].delta = Coord(0, 0, 0);
    scanners[initScanner].done = true;
    scanners[initScanner].tsi = 0;
    for (Coord c : scanners[initScanner].sets[0].coords)
    {
        beacons.insert(c);
    }

    bool done = false;
    while (!done)
    {
        bool nochange = true;

        for (int i = 0; i < scanners.size(); i++)
        {
            for (int j = 0; j < scanners.size(); j++)
            {
                if (CompareScanners(scanners[i], scanners[j]))
                {
                    nochange = false;
                }
            }
        }

        done = true;
        for (auto& s: scanners)
        {
            if (!s.done)
            {
                done = false;
            }
        }
        if (nochange)
        {
            printf("Exhasted all scanners\n");
            for (auto &s : scanners)
            {
                if (!s.done)
                {
                    printf("Scanner not done: %d\n", s.number);
                }
            }
            done = true;
        }
    }

    for (auto &s : scanners)
    {
        printf("Scanner: %d origin: [%d,%d,%d] done: %d\n", s.number,
               std::get<0>(s.delta), std::get<1>(s.delta), std::get<2>(s.delta), s.done);
    }

    //for (Coord c : beacons)
    //{
    //    printf("Beacon [%d,%d,%d]\n", std::get<0>(c), std::get<1>(c), std::get<2>(c));
    //}
    printf("Total Beacons: %lu\n", beacons.size());

    int maxMd = 0;
    for (int i = 0; i < scanners.size(); i++)
    {
        for (int j = 0; j < scanners.size(); j++)
        {
            if (i == j)
                continue;

            Coord d = Diff(scanners[i].delta, scanners[j].delta);
            int md = abs(std::get<0>(d)) + abs(std::get<1>(d)) + abs(std::get<2>(d));
            //printf("Manhattan distance: %d - %d, size: %d\n", i, j, l);
            if (md > maxMd)
            {
                maxMd = md;
            }
        }
    }

    printf("Largest Manhattan distance: %d\n", maxMd);
    
    return 0;
}