#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <algorithm>

int main(int argc, char **argv)
{
    if (argc < 6)
    {
        throw std::invalid_argument("too few cmd line arguments");
    }

    if (!strcmp(argv[1], "-s"))
    {
        // compress data from input file

        std::ifstream input_file(argv[3], std::ios::in);

        if (!input_file.is_open())
        {
            throw std::invalid_argument("input file not found");
        }

        std::ofstream output_file(argv[5], std::ios::out | std::ios::binary);

        if (!output_file.is_open())
        {
            throw std::invalid_argument("output file not found");
        }

        unsigned node_a, node_b, weight;

        std::unordered_map<unsigned, std::set<std::pair<unsigned, unsigned>>> graph;

        while (!input_file.eof())
        {
            input_file >> node_a >> node_b >> weight;
            graph[node_a].insert({node_b, weight});
            graph[node_b].insert({node_a, weight});
        }

        std::vector<std::pair<unsigned, unsigned>> nodes;

        for (auto &it : graph)
        {
            nodes.push_back({it.first, it.second.size()});
        }

        std::sort(nodes.begin(), nodes.end(), [](const std::pair<unsigned, unsigned> &a, const std::pair<unsigned, unsigned> &b)
                  { return a.second > b.second; });

        for (auto node : nodes)
        {
            if (graph[node.first].size() == 0)
            {
                // node has no edges
                continue;
            }

            // iterate over all nodes in graph from nodes with highest degree to lowest
            unsigned char tmp = graph[node.first].size();

            try
            {
                // output_file << node.first << node.second;//static_cast<char>(node.second);
                output_file.write(reinterpret_cast<char *>(&node.first), sizeof(node.first));
                output_file.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
            }
            catch (std::exception &e)
            {
                std::cout << e.what() << std::endl;
            }

            for (auto it : graph[node.first])
            {
                // iterate over all edges of current node
                tmp = it.second;

                try
                {
                    output_file.write(reinterpret_cast<char *>(&it.first), sizeof(it.first));
                    output_file.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
                }
                catch (std::exception &e)
                {
                    std::cout << e.what() << std::endl;
                }

                if (graph[it.first].find({node.first, it.second}) == graph[it.first].end())
                {
                    // edge not found in graph
                    throw std::invalid_argument("edge not found in graph");
                }
                else if (it.first != node.first)
                {
                    // remove edge from graph
                    graph[it.first].erase({node.first, it.second});
                }
            }
        }

        input_file.close();
        output_file.close();
    }
    else if (!strcmp(argv[1], "-d"))
    {
        // decompress data from input binary file

        std::ifstream input_file(argv[3], std::ios::in | std::ios::binary);
        std::ofstream output_file(argv[5], std::ios::out);

        std::unordered_map<unsigned, std::set<std::pair<unsigned, unsigned>>> graph;

        unsigned node_a, node_b;
        unsigned char n, weight;

        while (!input_file.eof())
        {
            // input_file >> node_a >> n;
            input_file.read(reinterpret_cast<char *>(&node_a), sizeof(node_a));
            input_file.read(reinterpret_cast<char *>(&n), sizeof(n));

            for (unsigned i = 0; i < n; i++)
            {
                // input_file >> node_b >> weight;t
                input_file.read(reinterpret_cast<char *>(&node_b), sizeof(node_b));
                input_file.read(reinterpret_cast<char *>(&weight), sizeof(weight));

                graph[node_a].insert({node_b, weight});
            }
        }

        for (auto &node : graph)
        {
            for (auto &edge : node.second)
            {
                output_file << node.first << " " << edge.first << " " << edge.second << std::endl;
            }
        }

        input_file.close();
        output_file.close();
    }

    return 0;
}
