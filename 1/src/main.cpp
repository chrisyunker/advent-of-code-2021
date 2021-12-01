#include <stdio.h>
#include <getopt.h>

int main(int argc, char* argv[])
{
    const int numArgs = 0;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    //const std::string deployment(argv[optind]);
    //const std::string puuid(argv[optind + 1]);

    printf("AoC\n");

    return 0;
}
