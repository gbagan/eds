#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "../include/Util.h"
#include "../include/Graph.h"
#include "../include/Arena.h"
#include "../include/Orientation.h"
#include "../include/Tournament.h"

#include "emscripten/system/include/emscripten/bind.h"

using namespace emscripten;

bool hasEternalDomSet(const Graph & graph, int k, Rules * rules) {
    Arena arena = Arena::computeArena(graph, k, rules);
    return arena.guardsWin();
}

int main() {
    auto rules = new StandardRules();
    Graph graph(4);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    std::cout << hasEternalDomSet(graph, 2, rules) << std::endl;
}
