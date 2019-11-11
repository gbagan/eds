//
// Created by Guillaume on 20/08/2017.
//

#include <stack>
#include <algorithm>
#include <memory>

#include "../include/Arena.h"
#include "../include/Util.h"

void Arena::addEdge(const std::vector<int> &from, const std::vector<int> & to) {
    adj[from].push_back(to);
    reverse_adj[to].push_back(from);
    adj[to];
    reverse_adj[from];
}


Arena Arena::computeArena(const Graph & graph, int k, const std::string & rulesName) {
    Arena arena (graph.noVertices(), k);
    std::unique_ptr<Rules> rules (Rules::makeRules(rulesName));

    auto confs = sublists(graph.noVertices(), k);

    for (const auto & guards : confs) {
        auto poss = rules->attackerPossibilities(graph, guards);
        for (auto pos : poss) {
            arena.addEdge(guards, pos);
        }

        for(int attack = 0; attack < graph.noVertices(); attack++) {
            if(std::count(guards.begin(), guards.end(), attack) == 0) {
                std::vector<int> conf (guards);
                conf.push_back(attack);
                auto poss = rules->guardsPossibilities(graph, conf);
                for (auto pos : poss) {
                    arena.addEdge(conf, pos);
                }
            }
        }
    }
    arena.computeAttractor();
    return arena;
}



void Arena::computeAttractor() {

    std::unordered_map<
            std::vector<int>,
            int,
            boost::hash<std::vector<int>>
    > deg;

    std::stack<std::vector<int>> stack;

    for (const auto & pair: adj) {
        deg[pair.first] = pair.second.size();
        if(pair.first.size() == k+1 && pair.second.size() == 0) { // final winning configurations for the attacker
            stack.push(pair.first);
            attractor.insert(pair.first);
        }
    }

    while(!stack.empty()) {
        auto elem = stack.top();
        stack.pop();

        for(const auto &  pred : reverse_adj.at(elem)) {
            if(attractor.count(pred) == 0 && (pred.size() == k || --deg[pred] == 0)) {
                attractor.insert(pred);
                stack.push(pred);
            }
        }
    }

    //attractor = std::move(marked);
}

bool Arena::guardsWin() const {
    auto lists = sublists(n, k);

    return std::any_of(lists.begin(), lists.end(), [this] (const auto & guards) {
        return this->attractor.count(guards) == 0;
    });

}

std::vector<int> Arena::startingConf() const {
    auto lists = sublists(n, k);
    for(const auto & conf : lists) {
        if (conf.size() == k && attractor.count(conf) == 0)
            return conf;
    }
    return std::vector<int>();
}

std::vector<int> Arena::answer(const std::vector<int> & conf) const {
    for(const auto & conf2 : adj.at(conf)) {
        if (conf2.size() == k && attractor.count(conf2) == 0)
            return conf2;
    }
    return std::vector<int>();
}