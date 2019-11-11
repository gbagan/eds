//
// Created by Guillaume on 27/08/2017.
//

#include "Tournament.h"
#include "../include/Util.h"

std::vector<bool> computeQuadraticResidues(int n) {
    std::vector<bool> qr (n);
    for (int i = 0; i < n; i++) {
        qr[(i *i) % n] = true;
    }
    return qr;
}

Graph paleyGraph(int n){
    Graph graph (n);

    auto qr = computeQuadraticResidues(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && qr[(n + i - j) % n]) {
                graph.addArc(i, j);
            }
        }
    }
    return graph;
}

Graph oddEvenGraph(int n) {
    Graph graph (n);

    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n; j++) {
            if (j % 2 == 1) {
                graph.addArc(i, (i+j) % n);
            }
        }
    }
    return graph;
}


Graph TransitiveTournaments::generateIth(int i) {
    int m = (n - 1) / 2;

    if (i < 0 || i > 1 << (m-1)) {
        throw std::out_of_range("generateIth");
    }

    auto bits = intToBinary(i, m);

    Graph graph (n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            graph.addArc(i, bits[j] ? (i+j+1) % n : (i+n-j-1) % n);
        }
    }

    return graph;
}
