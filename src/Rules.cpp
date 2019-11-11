//
// Created by Guillaume on 21/08/2017.
//

#include "Rules.h"

#include <algorithm>

Rules * Rules::makeRules(const std::string & name) {
    if (name == "standard")
        return new StandardRules();
    else if (name == "all")
        return new ManyRules();
    else
        return nullptr;
}

std::vector<std::vector<int>> StandardRules::guardsPossibilities(const Graph & g, const std::vector<int> & guards) {
    int attack = guards[guards.size()-1];
    auto nguards = std::vector<int>(guards.begin(), guards.end()-1);

    std::vector<std::vector<int>> possibilities;

    for (int i = 0; i < nguards.size(); i++) {
        if (g.hasArc(nguards[i], attack)) {
            std::vector<int> nguards2 (nguards);
            nguards2[i] = attack;
            possibilities.push_back(std::move(nguards2));
        }
    }

    for (auto & pos : possibilities) {
        std::sort(pos.begin(), pos.end());
    }

    return possibilities;
}


std::vector<std::vector<int>> ManyRules::guardsPossibilities(const Graph & g, const std::vector<int> & guards) {
    auto possibilities = StandardRules::guardsPossibilities(g, guards);

    int attack = guards[guards.size()-1];
    int k = guards.size()-1;

    for(int i = 0; i < k; i++) {
        int size = possibilities.size();
        for (int j = 0; j < size; j++) {
            auto pos = possibilities[j];
            if (pos[i] == attack)
                continue;
            for (int nbor : g.adj[pos[i]]) {
                possibilities.push_back(pos);
                possibilities.back()[i] = nbor;
            }
        }
    }

    for(auto & pos : possibilities) {
        std::sort(pos.begin(), pos.end());
        auto end = std::unique(pos.begin(), pos.end());
        pos.erase(end, pos.end());
    }
    // remove configurations with several guards on the same vertex
    auto end = std::remove_if(possibilities.begin(), possibilities.end(), [k] (const std::vector<int> & guards) {
        return guards.size() < k;
    });
    possibilities.erase(end, possibilities.end());

    return possibilities;
}

std::vector<std::vector<int>> StandardRules::attackerPossibilities(const Graph & g, const std::vector<int> & guards) {
    std::vector<std::vector<int>> possibilities;

    for(int attack = 0; attack < g.noVertices(); attack++) {
        if (std::count(guards.begin(), guards.end(), attack) == 0) {
            possibilities.push_back(guards);
            possibilities.back().push_back(attack);
        }
    }

    return possibilities;
}
