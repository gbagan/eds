from eternaldom import *

def makeClique(g, set):
    for i in range(len(set)-1):
        for j in range(i+1, len(set)):
            if not g.hasArc(set[i], set[j]):
                g.addEdge(set[i], set[j])

def makePath(g, set):
    for i in range(len(set)-1):
       if not g.hasArc(set[i], set[i+1]):
           g.addEdge(set[i], set[i+1])

def edges(g):
    for i in range(len(g)):
        for j in range(i+1, len(g)):
            if g.hasArc(i, j):
                yield (i, j)

def lineGraph(g):
    es = list(edges(g))
    m = {}
    g2 = graph(len(es))
    for i, e in enumerate(es):
       m[e] = i
    for i in range(len(g)):
       nbor = g.neighbors(i)
       for j in nbor:
           for k in nbor:
                if j >= k:
                   continue
                v1 = m[(i, j) if i < j else (j, i)]
                v2 = m[(i, k) if i < k else (k, i)]
                if not g2.hasArc(v1, v2):
                    g2.addEdge(v1, v2)
    return g2


def generateWindmill(n):
    g = graph(2*n+1)
    for i in range(n):
        makeClique(g, [2*i, 2*i+1, 2*n])
    return g

def generateTriangleChain(n):
    g = graph(2*n+1)
    for i in range(0, 2*n, 2):
        g.addEdge(i, i+1)
        g.addEdge(i+1, i+2)
        g.addEdge(i, i+2)
    return g

def generateTriClique(n, m, p):
    g = graph(n+m+p)
    for i in range(n):
        for j in range(n, n+m+p):
            g.addEdge(i, j)

    for i in range(n, n+m):
        for j in range(n+m, n+m+p):
            g.addEdge(i, j)

    return g


def generate22(n, m):
    g = graph(n*m)
    for i in range(n):
        for j in range(m):
            g.addEdge(i, (i+1)%n + j * n)
            if j > 0:
                g.addEdge(i + j * n, (i+1) % n)
    return g

def testGraph():
    g = graph(11)
    makeClique(g, [0, 1, 2])
    makeClique(g, [3, 4, 5])
    g.addEdge(2, 10)
    g.addEdge(10, 3)
    g.addEdge(0, 6)
    g.addEdge(1, 7)
    g.addEdge(4, 8)
    g.addEdge(5, 9)
    #makePath(g, [0, 3, 6, 9, 12])
    #makePath(g, [1, 4, 7, 10, 13])
    #makePath(g, [2, 5, 8, 11, 14])
    #makeClique(g, [0, 7, 8, 9])
    #makeClique(g, [0, 10, 11, 12])
    return g

def test2():
    g = graph(15)
    makePath(g, [0, 1, 2, 3])
    makePath(g, [5, 6, 2, 7, 8])
    makePath(g, [9, 10, 2, 11, 12])
    #g.addEdge(2, 7)
    #g.addEdge(2, 8)
    #makePath(g, [3, 11, 12])
    #makePath(g, [11, 13, 14])
    return lineGraph(g)

def test3():
    g = graph(6)
    makeClique(g, [0, 1, 2, 3])
    makeClique(g, [0, 4, 5])
#    makeClique(g, [0, 6, 7])
    print (g)
    return g

def test4():
    g = graph(13)
    makePath(g, [0, 1, 2, 0, 3, 1, 4, 0,
         5, 6, 0, 7, 5, 8, 0,
         9, 10, 0, 11, 9, 12, 0])
    
    return g
#digraph(13)
#.addPath(0, 1, 2, 0, 3, 1, 4, 0,
#         5, 6, 0, 7, 5, 8, 0,
#         9, 10, 0, 11, 9, 12, 0)

def printer(g):
    s = "digraph(%s).addEdges(" % len(g)
    for i in range(len(g)):
        for j in g.neighbors(i):
           s += "[%s, %s], " % (i, j)
    s += ')'
    return s

def main():
    g = test3()
    nb_guards = 3
    ors = orientations(g)

    nb = 0
    a =  30
    print("orientations: ", len(ors))
    rules = "all"

    for i, orient in enumerate(ors):
        step = len(ors) // 10
        if i % step == 0:
            print("--", i, a, (100*i)/len(ors), "%")
        #if not orient.trivialCheck():
        #    continue
        #a = min(a, orient.alpha())

        #if not orient.isEulerian():
        if not orient.trivialCheck():
             continue
        #print ("plop")
        if orient.hasEternalDomSet(nb_guards, rules):
            print(printer(orient))
    print ("a:", a)

main()

#9010000
