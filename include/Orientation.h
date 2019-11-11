//
// Created by Guillaume on 24/08/2017.
//

#ifndef ETERNALDOM_ORIENTATION_H
#define ETERNALDOM_ORIENTATION_H

#include <memory>

#include "Graph.h"

class OrientationGenerator {

private:
    enum MatrixCellType { Empty, Fixed, Variable };
    std::vector<std::vector<MatrixCellType>> templateMat;
    std::vector<std::vector<bool>> orientedMat;
    int nbVariables;

public:
    OrientationGenerator (const std::string & name);
    OrientationGenerator (const Graph & graph);
    unsigned long nbIterations() {  return (unsigned long) 1 << (unsigned long) nbVariables; }
    Graph computeIthOrientation(int i);

private:
    Graph toGraph() const;
    void computeNumberOfVariables();
    void initializeOrientation();
    void putVariable(int i, int j);

    void generateClique(int n);
    void generateBiclique(int n, int m);
    void generateGrid(int n, int m);
    void generatePetersen();
    void generateHajos(int n);
    void generateTorusGrid(int n, int m);
};

#endif //ETERNALDOM_ORIENTATION_H
