//
// Created by Guillaume on 24/08/2017.
//

#include "../include/Orientation.h"
#include "../include/Util.h"

#include <regex>

Graph OrientationGenerator::toGraph() const {
    Graph g (orientedMat.size());
    for (int i = 0; i < orientedMat.size(); i++) {
        for (int j = 0; j < orientedMat.size(); j++) {
            if(orientedMat[i][j]) {
                g.addArc(i, j);
            }
        }
    }
    return g;
}

void OrientationGenerator::computeNumberOfVariables()  {
    int nb = 0;
    for (auto & line : templateMat) {
        for (auto x : line) {
            if (x == Variable)
                nb++;
        }
    }
    nbVariables = nb;
}


void OrientationGenerator::initializeOrientation() {
    int n = templateMat.size();
    orientedMat = std::vector<std::vector<bool>>(n, std::vector<bool>(n, false));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (templateMat[i][j] == Fixed) {
                orientedMat[i][j] = true;
            }
        }
    }
}



Graph OrientationGenerator::computeIthOrientation(int i) {
    if (i < 0 || i >= nbIterations())
        throw std::out_of_range("OrientationGenerator");

    int n = templateMat.size();
    auto bin = intToBinary(i, nbVariables);
    int kk = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (templateMat[i][j] == Variable) {
                if (bin[kk]) {
                    orientedMat[i][j] = true;
                    orientedMat[j][i] = false;
                } else {
                    orientedMat[j][i] = true;
                    orientedMat[i][j] = false;
                }
                kk++;
            }
        }
    }
    return toGraph();
}


void OrientationGenerator::putVariable(int i, int j) {
    if(i < j) {
        templateMat[i][j] = Variable;
    } else {
        templateMat[j][i] = Variable;
    }
}

void OrientationGenerator::generateClique(int n) {
    templateMat = std::vector<std::vector<MatrixCellType>>(n, std::vector<MatrixCellType>(n, Empty));
    // we can assume that the orientation is strongly connected and thus hamiltonian.
    // Consequently, we can fix the cycle 0 -> 1 -> 2 ... -> n-1 -> 0
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            templateMat[i][j] = (j == i + 1) ?  Fixed : Variable;
        }
    }
    templateMat[0][n-1] = Empty;
    templateMat[n-1][0] = Fixed;
}


void OrientationGenerator::generateBiclique(int n, int m) {
    templateMat = std::vector<std::vector<MatrixCellType>>(n+m, std::vector<MatrixCellType>(n+m, Empty));

    for (int i = 0; i < n; i++) {
        for (int j = n; j < n+m; j++) {
            templateMat[i][j] = Variable;
        }
    }
    templateMat[0][n] = Fixed;
    templateMat[n][1] = Fixed;
    templateMat[1][n] = Empty;
}

void OrientationGenerator::generateGrid(int n, int m) {
    templateMat = std::vector<std::vector<MatrixCellType>>(n*m, std::vector<MatrixCellType>(n*m, Empty));

    for (int i = 0; i < n*m-1; i++) {
        if(i % m < m - 1)
            templateMat[i][i+1] = Variable;
        if(i < n*m-m)
            templateMat[i][i+m] = Variable;
    }

    templateMat[0][1] = Empty;
    templateMat[1][0] = Fixed;
    templateMat[0][m] = Fixed;
}


void OrientationGenerator::generateTorusGrid(int n, int m) {
    generateGrid(n, m);

    for (int i = 0; i < n*m-1; i += m) {
        templateMat[i][i+m-1] = Variable;
    }

    for (int i = 0; i < m; i++) {
        templateMat[i][i+m*(n-1)] = Variable;
    }
}


void OrientationGenerator::generateHajos(int n) {
    templateMat = std::vector<std::vector<MatrixCellType>>(2*n, std::vector<MatrixCellType>(2*n, Empty));

    for (int i = 0; i < n; i++) {
        putVariable(i,(i+1)%n);
        putVariable(i, i+n);
        putVariable((i+1)%n, i+n);
    }
    templateMat[0][1] = Fixed;
}


OrientationGenerator::OrientationGenerator (const std::string & name) {
    std::regex cliqueRegex (R"(K(\d+))");
    std::regex bicliqueRegex (R"(K(\d+),(\d+))");
    std::regex gridRegex (R"(G(\d+),(\d+))");
    std::regex torusRegex (R"(T(\d+),(\d+))");
    std::smatch matches;

    if (std::regex_match(name, matches, cliqueRegex)) {
        int n = std::stoi(matches[1]);
        generateClique(n);
    } else if (std::regex_match(name, matches, bicliqueRegex)) {
        int n = std::stoi(matches[1]);
        int m = std::stoi(matches[2]);
        generateBiclique(n, m);
    } else if (std::regex_match(name, matches, gridRegex)) {
        int n = std::stoi(matches[1]);
        int m = std::stoi(matches[2]);
        generateGrid(n, m);
    } else if (std::regex_match(name, matches, torusRegex)) {
        int n = std::stoi(matches[1]);
        int m = std::stoi(matches[2]);
        generateTorusGrid(n, m);
    } else {
        OrientationGenerator(Graph::generateGraph(name));
    }
    computeNumberOfVariables();
    initializeOrientation();
}

OrientationGenerator::OrientationGenerator (const Graph & graph) {
    int n = graph.noVertices();
    templateMat = std::vector<std::vector<MatrixCellType>>(n, std::vector<MatrixCellType>(n, Empty));
    for (int i = 0; i < n; i++) {
        for (int j : graph.adj[i]) {
            if (i < j) {
                templateMat[i][j] = Variable;
            }
        }
    }

    computeNumberOfVariables();
    initializeOrientation();
}