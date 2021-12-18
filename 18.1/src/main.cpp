#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>


struct Pair
{
    bool xreg = false;
    Pair* xp;
    int x;
    bool yreg = false;
    Pair* yp;
    int y;
};

void PrintPair(Pair* p, int depth = 0)
{
    printf("[");
    if (p->xreg)
    {
        printf("%d", p->x);
    }
    else
    {
        PrintPair(p->xp, depth+1);
    }
    printf(",");
    if (p->yreg)
    {
        printf("%d", p->y);
    }
    else
    {
        PrintPair(p->yp, depth+1);
    }

    printf("]");
    if (depth == 0)
        printf("\n");
}

struct Explode
{
    bool found = false;
    bool explode = false;
    int x = 0;
    int y = 0;
};

bool AddLeft(Pair* p, int value)
{
    if (value == 0)
        return true;

    bool done = false;
    if (p->yreg)
    {
        p->y += value;
        return true;
    }
    else
    {
        done = AddLeft(p->yp, value);
    }
    if (done)
        return done;
    if (p->xreg)
    {
        p->x += value;
        return true;
    }
    else
    {
        done = AddLeft(p->xp, value);
    }
    return done;
}

bool AddRight(Pair* p, int value)
{
    if (value == 0)
        return true;
        
    bool done = false;
    if (p->xreg)
    {
        p->x += value;
        return true;
    }
    else
    {
        done = AddRight(p->xp, value);
    }
    if (done)
        return done;
    if (p->yreg)
    {
        p->y += value;
        return true;
    }
    else
    {
        done = AddRight(p->yp, value);
    }
    return done;
}

Explode CheckExplode(Pair* p, int depth = 0)
{
    Explode e{};

    if (depth >= 4 &&
        p->xreg &&
        p->yreg)
    {
        //printf("EXPLODE: [%d,%d]\n", p->x, p->y);
        e.found = true;
        e.explode = true;
        e.x = p->x;
        e.y = p->y;
        return e;
    }

    if (!p->xreg)
    {
        e = CheckExplode(p->xp, depth+1);
        if (e.explode)
        {
            delete p->xp;
            p->xreg = true;
            p->x = 0;
            e.explode = false;
        }
    }
    if (e.y > 0)
    {
        if (p->yreg)
        {
            p->y += e.y;
            e.y = 0;
        }
        else
        {
            if (AddRight(p->yp, e.y))
            {
                e.y = 0;
            }
        }
    }
    if (e.found)
        return e;
    if (!p->yreg)
    {
        e = CheckExplode(p->yp, depth+1);
        if (e.explode)
        {
            delete p->yp;
            p->yreg = true;
            p->y = 0;
            e.explode = false;
        }
    }
    if (e.x > 0)
    {
        if (p->xreg)
        {
            p->x += e.x;
            e.x = 0;
        }
        else
        {
            if (AddLeft(p->xp, e.x))
            {
                e.x = 0;
            }
        }
    }
    return e;
}

bool CheckSplit(Pair* p)
{
    bool found = false;
    if (p->xreg && p->x > 9)
    {
        //printf("SPLIT X: %d\n", p->x);
        p->xp = new Pair{};
        p->xp->x = p->x / 2;
        p->xp->xreg = true;
        p->xp->y = (p->x / 2) + (p->x % 2);
        p->xp->yreg = true;
        p->xreg = false;
        p->x = 0;
        return true;
    }
    else if (!p->xreg)
    {
        found = CheckSplit(p->xp);
    }
    if (found)
        return found;

    if (p->yreg && p->y > 9)
    {
        //printf("SPLIT Y: %d\n", p->y);
        p->yp = new Pair{};
        p->yp->x = p->y / 2;
        p->yp->xreg = true;
        p->yp->y = (p->y / 2) + (p->y % 2);
        p->yp->yreg = true;
        p->yreg = false;
        p->y = 0;
        return true;
    }
    else if (!p->yreg)
    {
        found = CheckSplit(p->yp);
    }
    return found;
}

Pair* AddPair(Pair* a, Pair* b)
{
    Pair* p = new Pair{};
    p->xp = a;
    p->yp = b;
    return p;
}

void Reduce(Pair* p)
{
    bool done = false;
    while (!done)
    {
        done = true;

        Explode e;
        do
        {
            e = CheckExplode(p);
            if (e.found)
            {
                //printf("EXPLODE\n");
                //PrintPair(p);
                done = false;
            }
        } while (e.found);

        bool split = CheckSplit(p);
        if (split)
        {
            // printf("SPLIT\n");
            //PrintPair(p);
            done = false;
        }
    }
    //PrintPair(p);
}

int Magnitude(Pair* p)
{
    int left = 0;
    if (!p->xreg)
    {
        left = Magnitude(p->xp);
    }
    else
    {
        left = p->x;
    }

    int right = 0;
    if (!p->yreg)
    {
        right = Magnitude(p->yp);
    }
    else
    {
        right = p->y;
    }

    return (left * 3) + (right * 2);
}

Pair* ParseLine(const std::string& line, int& ptr)
{
    Pair* p = nullptr;
    if (line[ptr++] == '[')
    {
        p = new Pair{};
        if (line[ptr] == '[')
        {
            p->xp = ParseLine(line, ptr);
        }
        else
        {
            int next = line.find_first_of(",", ptr);
            p->x = std::stoi(line.substr(ptr, next-ptr));
            p->xreg = true;
            ptr = next;
        }
        ptr++; // ','
        if (line[ptr] == '[')
        {
            p->yp = ParseLine(line, ptr);
        }
        else
        {
            int next = line.find_first_of("]", ptr);
            p->y = std::stoi(line.substr(ptr, next-ptr));
            p->yreg = true;
            ptr = next;
        }
        ptr++; // ']'
    }
    return p;
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

    std::vector<Pair*> pairs;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        int ptr = 0;
        Pair* p = ParseLine(line, ptr);
        pairs.push_back(p);
    }
    printf("Number of pairs: %lu\n", pairs.size());

    Pair* sum = AddPair(pairs[0], pairs[1]);
    Reduce(sum);

    for (int i = 2; i < pairs.size(); i++)
    {
        sum = AddPair(sum, pairs[i]);
        Reduce(sum);
    }

    int mag = Magnitude(sum);
    printf("Magnitude: %d\n", mag);

    return 0;
}