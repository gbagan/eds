//
// Created by Guillaume on 20/08/2017.
//

#ifndef CODE_GAMEGRAPH_H
#define CODE_GAMEGRAPH_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <boost/functional/hash.hpp>

#include "Rules.h"
#include "Graph.h"

class Arena {
public:
    typedef std::unordered_map<
            std::vector<int>,
            std::vector<std::vector<int>>,
            boost::hash<std::vector<int>>
    > adj_t;
    typedef std::unordered_set<
            std::vector<int>,
            boost::hash<std::vector<int>>
    > vertices_set_t;

    adj_t adj;
    adj_t reverse_adj;

    int n;   // number of vertices in the initial graph
    int k;   // number of guards

    Arena(int nn, int kk) : k(kk), n(nn) {};

    static Arena computeArena(const Graph & graph, int k, const std::string & rules);

    void addEdge(const std::vector<int> & from, const std::vector<int> & to);

    bool guardsWin() const;
    std::vector<int> startingConf() const;
    std::vector<int> answer(const std::vector<int> & conf) const;

private:
    Arena::vertices_set_t attractor;
    void computeAttractor();
};


#endif //CODE_GAMEGRAPH_H
