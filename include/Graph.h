//
// Created by Guillaume on 19/08/2017.
//

#ifndef CODE_SIMPLEGRAPH_H
#define CODE_SIMPLEGRAPH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <memory>

// A class that represents a simple directed graph
class Graph
{
    int V;    // No. of vertices

public:
    std::vector<std::vector<int>> adj;    // A dynamic array of adjacency lists
    std::vector<std::vector<int>> reverse_adj;

    inline int noVertices() const { return V; }

    Graph(int V);   // Constructor
    Graph(const std::string & name) : Graph(generateGraph(name)) {};

    void addArc(int v, int w);
    bool hasArc(int v, int w) const;
    void addEdge(int v, int w);
    void removeArc(int v, int w);
    void removeEdge(int v, int w);

    void addEdges(const std::vector<std::pair<int,int>> & edges);
    void makePath(const std::vector<int> & list);
    void makeCycle(const std::vector<int> & list);
    void makeClique(const std::vector<int> & list);


    Graph reverse() const;
    Graph inducedGraph(const std::vector<int> & set) const;
    bool isAcyclic() const;
    int alpha(bool reverse = false) const;
    int mis() const;

    bool isIsomorphicTo(const Graph & g) const;

    bool trivialCheck() const;

    bool isConnected() const ;
    bool isEulerian() const;


    static Graph readFromFile(const std::string & fileName);
    void writeToFile(const std::string & fileName);
    static Graph generateGraph(const std::string & name);

private:
    bool isIsomorphicToAux(const Graph & g, const std::vector<int> & perm) const;
    int alphaAux() const;
    int alphaReverse() const;

    static Graph generateClique(int n);
    static Graph generateBiclique(int n, int m);
    static Graph generateGrid(int n, int m);
    static Graph generateTorus(int n, int m);
    static Graph generatePetersen();
    static Graph generateHajos(int n);
};

std::ostream & operator << (std::ostream & os, const Graph & graph);

#endif //CODE_SIMPLEGRAPH_H
