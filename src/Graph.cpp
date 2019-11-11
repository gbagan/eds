//
// Created by Guillaume on 19/08/2017.
//

#include "../include/Graph.h"
#include "../include/Util.h"

#include <stack>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <regex>
#include <cmath>

Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
    reverse_adj.resize(V);
}


void Graph::addArc(int v, int w) {
    adj[v].push_back(w);
    reverse_adj[w].push_back(v);
}

bool Graph::hasArc(int v, int w) const {
    return std::count(adj[v].begin(), adj[v].end(), w) > 0;
}

void Graph::addEdge(int v, int w) {
    addArc(v, w);
    addArc(w, v);
}

void Graph::removeArc(int v, int w) {
    auto end = std::remove(adj[v].begin(), adj[v].end(), w);
    adj[v].erase(end, adj[v].end());
    end = std::remove(reverse_adj[w].begin(), reverse_adj[w].end(), v);
    reverse_adj[w].erase(end, reverse_adj[w].end());
}

void Graph::removeEdge(int v, int w) {
    removeArc(v, w);
    removeArc(w, v);
}

std::ostream & operator << (std::ostream & os, const Graph & graph) {
    for(int i = 0; i < graph.noVertices(); i++) {
        os << i << ":";
        for(int neighbor : graph.adj[i]) {
            os <<  " " << neighbor;
        }
        os << std::endl;
    }
    return os;
}

/*
bool Graph::isIsomorphicToAux(const Graph & g, const std::vector<int> & perm) const {
    for (int i = 0; i < V; i++) {
        if(adj[perm[i]].size() != g.adj[i].size()) {
            return false;
        }
        std::unordered_set<int> nbors;
        for (int nbor : adj[perm[i]]) {
            nbors.insert(perm[nbor]);
        }
        if (nbors != g.adj[i])
            return false;
    }
    return true;
}

bool Graph::isIsomorphicTo(const Graph & g) const {
    if(noVertices() != g.noVertices()) {
        return false;
    }

    std::vector<int> perm(V);
    for (int i = 0; i < V; i++) {
        perm[i] = i;
    }

    do {
        if (isIsomorphicToAux(g, perm)) return true;
    } while(std::next_permutation(perm.begin(), perm.end()));

    return false;
}
*/

Graph Graph::reverse() const {
    Graph g (noVertices());
    g.adj = reverse_adj;
    g.reverse_adj = adj;
    return g;
}

Graph Graph::inducedGraph(const std::vector<int> & set) const {
    Graph g(set.size());
    std::unordered_map<int,int> reverse;

    for (int i = 0; i < set.size(); i++) {
        reverse[set[i]] = i;
    }

    for (int i = 0; i < set.size(); i++) {
        for(auto nbor : adj[set[i]]) {
            auto it = reverse.find(nbor);
            if (it != reverse.end()) {
                g.addArc(i, it->second);
            }
        }
    }
    return g;
}

bool Graph::isAcyclic() const {
    std::vector<bool> visited(V, false);
    std::vector<int> indeg(V);
    std::queue<int> queue;

    for (int i = 0; i < V; i++) {
        indeg[i] = reverse_adj[i].size();
        if (indeg[i] == 0) {
            queue.push(i);
            visited[i] = true;
        }
    }

    while (!queue.empty()) {
        int v = queue.front();
        queue.pop();
        for(int nbor : adj[v]) {
            if (!visited[nbor] && --indeg[nbor] == 0) {
                queue.push(nbor);
                visited[nbor] = true;
            }
        }
    }

    return std::find(visited.begin(), visited.end(), false) == visited.end();
}

int Graph::alpha(bool reverse) const {
    if (reverse)
        return alphaReverse();
    else
        return alphaAux();
}


int Graph::alphaAux() const {
    std::vector<std::vector<int>> slists;
    slists.push_back(std::vector<int>());

    int i = 0;
    while(true) {
        bool findAcyclic = false;
        std::vector<std::vector<int>> slists2;

        for (auto slist : slists) {
            int size = slist.size();
            int begin = size == 0 ? 0 : slist.back() + 1;
            for (int ii = begin; ii < V; ii++) {
                std::vector<int> slist2(slist);
                slist2.push_back(ii);
                if (inducedGraph(slist2).isAcyclic()) {
                    slists2.push_back(slist2);
                    findAcyclic = true;
                }
            }
        }
        slists = std::move(slists2);
        if (!findAcyclic)
            return i;

        i++;
    }
}

int Graph::mis() const {
    std::vector<std::vector<int>> slists;
    slists.push_back(std::vector<int>());

    int i = 0;
    while(true) {
        bool findMIS = false;
        std::vector<std::vector<int>> slists2;

        for (auto slist : slists) {
            int size = slist.size();
            int begin = size == 0 ? 0 : slist.back() + 1;
            for (int ii = begin; ii < V; ii++) {
                bool testIS = true;
                for (auto jj : slist) {
                    if(hasArc(ii, jj)) {
                        testIS = false;
                        break;
                    }
                }
                if (testIS) {
                    std::vector<int> slist2(slist);
                    slist2.push_back(ii);
                    slists2.push_back(slist2);
                    findMIS = true;
                }
            }
        }
        slists = std::move(slists2);
        if (!findMIS)
            return i;

        i++;
    }
}


int Graph::alphaReverse() const {
    for (int i = V; i > 0; i--) {
        auto slists = sublists(V, i);

        for (const auto & slist : slists) {
            if (inducedGraph(slist).isAcyclic()) {
                return i;
            }
        }
    }
    return 0;
}

bool Graph::trivialCheck() const {
    int n = V;
    return !std::any_of(adj.begin(), adj.end(), [n] (const auto & l) {
        return l.size() == 0 || l.size() == n-1;
    });
}

bool Graph::isConnected() const { // assume that G is a non directed graph
    std::vector<bool> visited(V);
    std::stack<int> stack;
    int nb_visited = 0;
    stack.push(0);
    while(!stack.empty()) {
        int u = stack.top();
        stack.pop();
        visited[u] = true;
        nb_visited++;
        for (int w : adj[u]) {
            if (!visited[w]) {
                stack.push(w);
            }
        }
    }

    return nb_visited == V;
}

bool Graph::isEulerian() const {
    for (int i = 0; i < V; i++) {
        if (std::abs((int) adj[i].size() - (int) reverse_adj[i].size()) >= 2) {
            return false;
        }
    }
    return true;
}




Graph Graph::readFromFile(const std::string & fileName) {
    std::string line;
    std::ifstream graphFile { fileName };

    std::smatch matches;
    std::regex nPat (R"(n,(\d+))");
    std::regex edgePat (R"((\d+),(\d+))");

    std::getline(graphFile, line);
    std::regex_search(line, matches, nPat);
    int n = std::stoi(matches[1]);
    Graph graph(n);

    while(std::getline(graphFile, line)) {
        if(std::regex_search(line, matches, edgePat)) {
            int x = std::stoi(matches[1]);
            int y = std::stoi(matches[2]);
            graph.addArc(x, y);
        }
    }
    graphFile.close();
    return graph;
};


void Graph::writeToFile(const std::string & fileName) {
    std::ofstream graphFile { fileName };
    graphFile << "n," << V << "\n";
    for (int i = 0; i < V; i++) {
        for (int j : adj[i]) {
            graphFile << i << "," << j << "\n";
        }
    }
    graphFile.close();
}


Graph Graph::generateGraph(const std::string & name) {
    std::regex cliqueRegex (R"(K(\d+))");
    std::regex bicliqueRegex (R"(K(\d+),(\d+))");
    std::regex gridRegex (R"(G(\d+),(\d+))");
    std::regex hajosRegex (R"(hajos(\d+))");
    std::regex torusRegex (R"(T(\d+),(\d+))");
    std::smatch matches;

    if (std::regex_match(name, matches, cliqueRegex)) {
        int n = std::stoi(matches[1]);
        return generateClique(n);
    } else if (std::regex_match(name, matches, bicliqueRegex)) {
        int n = std::stoi(matches[1]);
        int m = std::stoi(matches[2]);
        return generateBiclique(n, m);
    } else if (std::regex_match(name, matches, gridRegex)) {
        int n = std::stoi(matches[1]);
        int m = std::stoi(matches[2]);
        return generateGrid(n, m);
    } else if (name == "petersen") {
        return generatePetersen();
    } else if (name == "hajos") {
        return generateHajos(3);
    } else if (std::regex_match(name, matches, hajosRegex)) {
        int n = std::stoi(matches[1]);
        return generateHajos(n);
    } else if (std::regex_match(name, matches, torusRegex)) {
        int n = std::stoi(matches[1]);
        int m = std::stoi(matches[2]);
        return generateTorus(n, m);
    } else {
        return Graph(0); // todo
    }
}

Graph Graph::generateClique(int n) {
    Graph g(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            g.addEdge(i, j);
        }
    }
    return g;
}

Graph Graph::generateBiclique(int n, int m) {
    Graph g(n+m);
    for (int i = 0; i < n; i++) {
        for (int j = n; j < n+m; j++) {
            g.addEdge(i, j);
        }
    }
    return g;
}

Graph Graph::generateGrid(int n, int m) {
    Graph g(n * m);
    for (int i = 0; i < n*m-1; i++) {
        if(i % m < m - 1)
            g.addEdge(i, i+1);
        if(i < n*m-m)
            g.addEdge(i, i+m);
    }
    return g;
}


Graph Graph::generateTorus(int n, int m) {
    Graph g = generateGrid(n, m);
    for (int i = 0; i < n*m-1; i += m)
        g.addEdge(i, i+m-1);

    for (int i = 0; i < m; i++)
        g.addEdge(i, i + m * (n - 1));
    return g;
}

Graph Graph::generatePetersen() {
    Graph g (10);

    for (int i = 0; i < 5; i++) {
        g.addEdge(i, (i + 1) % 5);
        g.addEdge(i + 5, (i + 2) % 5 + 5);
        g.addEdge(i, i + 5);
    }

    return g;
}

Graph Graph::generateHajos(int n) {
    Graph g (2*n);
    for (int i = 0; i < n; i++) {
        g.addEdge(i, (i+1)%n);
        g.addEdge(i, i+n);
        g.addEdge((i+1)%n, i+n);
    }
    return g;
}
