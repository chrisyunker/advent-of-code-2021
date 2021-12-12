#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <set>
#include <map>

struct Path
{
    std::set<std::string> smallNodes;
    std::vector<std::string> nodes;
};

struct Node
{
    std::string name;
    std::set<std::string> out;
    bool large = false;
};

typedef std::map<std::string, Node> Graph;


bool largeNode(std::string name)
{
    for(char c : name)
    {
        if (std::islower(c))
        {
            return false;
        }
    }
    return true;
}

void parseInput(std::ifstream &fd, Graph& graph)
{
    std::string del = "-";

    std::string line;
    while (std::getline(fd, line))
    {
        size_t pos;
        size_t prev = 0;
        std::string start, end;
        while ((pos = line.find_first_of(del, prev)) != std::string::npos)
        {
            if (pos > prev)
            {
                start = line.substr(prev, pos - prev);
            }
            prev = pos + 1;
        }
        if (prev < line.length())
        {
            end = line.substr(prev, std::string::npos);
        }

        if (graph.contains(start))
        {
            graph[start].out.insert(end);
        }
        else
        {
            Node node;
            node.name = start;
            node.large = largeNode(start);
            node.out.insert(end);
            graph[start] = node;
        }

        if (graph.contains(end))
        {
            graph[end].out.insert(start);
        }
        else
        {
            Node node;
            node.name = end;
            node.large = largeNode(end);
            node.out.insert(start);
            graph[end] = node;
        }
    }
};

void printPath(Path& p)
{
    for (int i = 0; i < (p.nodes.size() - 1); i++)
    {
        printf("%s,", p.nodes[i].c_str());
    }
    printf("%s", p.nodes[p.nodes.size() - 1].c_str());
    printf("\n");
}

void search(std::string node, Graph& graph, Path path, std::vector<Path>& paths)
{
    if (path.smallNodes.contains(node))
    {
        return;
    }

    if (!graph[node].large)
    {
        path.smallNodes.insert(node);
    }

    path.nodes.push_back(node);

    if (node == "end")
    {
        paths.push_back(path);
        return;
    }

    for (auto outNode : graph[node].out)
    {
        search(outNode, graph, path, paths);
    }
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

    Graph graph;

    parseInput(fd, graph);

    std::vector<Path> paths;
    
    search("start", graph, Path{}, paths);

    for (Path path : paths)
    {
        printPath(path);
    }
    printf("Found %lu paths\n", paths.size());

    return 0;
}