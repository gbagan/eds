//
// Created by Guillaume on 21/08/2017.
//

#ifndef ETERNALDOM_RULE_H
#define ETERNALDOM_RULE_H

#include <vector>
#include <memory>
#include "Graph.h"

class Rules {
public:
    virtual std::vector<std::vector<int>> guardsPossibilities(const Graph & g, const std::vector<int> & guards) = 0;
    virtual std::vector<std::vector<int>> attackerPossibilities(const Graph & g, const std::vector<int> & guards) = 0;
    static Rules * makeRules(const std::string & name);
};

class StandardRules : public Rules {
public:
    std::vector<std::vector<int>> guardsPossibilities(const Graph & g, const std::vector<int> & guards);
    std::vector<std::vector<int>> attackerPossibilities(const Graph & g, const std::vector<int> & guards);
};

class ManyRules : public StandardRules {
public:
    std::vector<std::vector<int>> guardsPossibilities(const Graph & g, const std::vector<int> & guards);
};



#endif //ETERNALDOM_RULE_H
