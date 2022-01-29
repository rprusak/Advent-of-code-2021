#include <iostream>
#include <list>
#include <sstream>

#include "common.h"

class Node : public std::string {
  public:
    explicit Node(const std::string &s) : std::string(s) {}

    [[nodiscard]] bool isEnd() const { return *this == "end"; }

    [[nodiscard]] bool isStart() const { return *this == "start"; }

    [[nodiscard]] bool isSmall() const { return std::all_of(this->begin(), this->end(), islower); }
};

class Path : public std::list<Node> {
  public:
    using std::list<Node>::list;

    bool containsNode(const Node &node) {
        return std::find(this->begin(), this->end(), node) != this->end();
    }

    bool containsDoubleSmallNode() {
        for (auto i = this->begin(); i != this->end(); i++) {
            auto t = i;
            t++;
            for (auto j = t; j != this->end(); j++) {
                if (*i == *j && i->isSmall()) return true;
            }
        }

        return false;
    }

    std::string toString() const {
        std::stringstream ss;

        for (const auto &node : *this) ss << node << "-";

        return ss.str();
    }
};

using Edge = std::pair<Node, Node>;

class Graph : public std::vector<Edge> {
  public:
    std::vector<Node> getCloseNodes(const Node &n) const {
        std::vector<Node> closeNodes;

        for (const auto &edge : *this) {
            if (edge.first == n)
                closeNodes.push_back(edge.second);
            else if (edge.second == n)
                closeNodes.push_back(edge.first);
        }

        return closeNodes;
    }
};

Edge parseLine(const std::string &str) {
    const auto splitLine = common::split(str, "-");
    return Edge{splitLine[0], splitLine[1]};
}

Graph parseInput(const std::vector<std::string> &input) {
    Graph graph{};

    for (const auto &line : input) graph.push_back(parseLine(line));

    return graph;
}

namespace part1 {
void buildNextLevelPath(const Graph &graph, Path currentPath, std::vector<Path> &paths) {
    const auto closeNodes = graph.getCloseNodes(currentPath.back());

    for (const auto &node : closeNodes) {
        if (node.isEnd()) {
            Path finalPath{currentPath};
            finalPath.push_back(node);
            paths.push_back(finalPath);
        } else if (node.isSmall()) {
            if (currentPath.containsNode(node)) continue;

            Path nextPath(currentPath);
            nextPath.push_back(node);
            buildNextLevelPath(graph, nextPath, paths);
        } else {
            Path nextPath(currentPath);
            nextPath.push_back(node);
            buildNextLevelPath(graph, nextPath, paths);
        }
    }
}

unsigned calculateDistinctPaths(const Graph &graph) {
    std::vector<Path> paths;

    Path startPath;
    startPath.push_back(Node{"start"});

    buildNextLevelPath(graph, startPath, paths);

    return paths.size();
}
}  // namespace part1

namespace part2 {
void buildNextLevelPath(const Graph &graph, Path currentPath, std::vector<Path> &paths) {
    const auto closeNodes = graph.getCloseNodes(currentPath.back());

    for (const auto &node : closeNodes) {
        if (node.isStart()) {
            continue;
        } else if (node.isEnd()) {
            Path finalPath{currentPath};
            finalPath.push_back(node);
            paths.push_back(finalPath);
        } else if (node.isSmall()) {
            if (currentPath.containsNode(node) && currentPath.containsDoubleSmallNode()) continue;

            Path nextPath(currentPath);
            nextPath.push_back(node);
            buildNextLevelPath(graph, nextPath, paths);
        } else {
            Path nextPath(currentPath);
            nextPath.push_back(node);
            buildNextLevelPath(graph, nextPath, paths);
        }
    }
}

unsigned calculateDistinctPaths(const Graph &graph) {
    std::vector<Path> paths;

    Path startPath;
    startPath.push_back(Node{"start"});

    buildNextLevelPath(graph, startPath, paths);

    //    for (const auto& p : paths)
    //        std::cout << p.toString() << std::endl;

    return paths.size();
}
}  // namespace part2

int main(int argc, char **argv) {
    try {
        const auto inputFilename = common::getFilename(argc, argv);
        const auto input = common::readInput(inputFilename);
        const auto graph = parseInput(input);

        std::cout << "day 1: " << part1::calculateDistinctPaths(graph) << std::endl;
        std::cout << "day 2: " << part2::calculateDistinctPaths(graph) << std::endl;

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}