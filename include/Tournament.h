//
// Created by Guillaume on 27/08/2017.
//

#ifndef ETERNALDOM_PALEY_H
#define ETERNALDOM_PALEY_H

#include <memory>
#include <vector>
#include "Graph.h"

std::vector<bool> computeQuadraticResidues(int n);
Graph paleyGraph(int n);
Graph oddEvenGraph(int n);


class TransitiveTournaments {
public:
    int n;
    TransitiveTournaments(int nn) : n(nn) {}

    int nbIterations() {
        return 1 << ((n - 1) / 2 - 1);
    }

    Graph generateIth(int i);
};


#endif //ETERNALDOM_PALEY_H
