import sys
from eternaldom import *

def main():
    name = sys.argv[1]
    rules = sys.argv[2]
    nb_guards = int(sys.argv[3]) 
    nb = 0
    ors = orientations(name)
    g = None
    for i, graph in enumerate(ors):
        if i % 100000 == 0:
            print("--", i)
        #if not graph.isEulerian():
        if not graph.trivialCheck():
            continue
        if graph.hasEternalDomSet(nb_guards, rules):
            g = graph
            break
            #graph_to_graphviz(graph, str(i))
    #print("number of solutions:", nb)
    if g is None:
        print("no solution")
        return
    g.save("data/%s_%s.graph" % (name, rules))
    fd = open("data/%s_%s.guards" % (name, rules), "w")
    fd.write(str(nb_guards))
    fd.close()
	
def main2():
    n = int(sys.argv[1])
    k = int(sys.argv[2])
    print ("n =", n)
    print ("k =", k)
    tts = transitivetournaments(n)
    gs = []
    print ("number of iterations:", len(tts))
    for i, tt in enumerate(tts):
        if tt.hasEternalDomSet(k, "standard"):
            # gs.append( tt)
            print(tt)
        #print("i:", i, tt.neighbors(0) ,"alpha =", tt.alpha(), "<=" + str(k) if tt.hasEternalDomSet(k, "standard") else "")
    #print gs[]
    #gs[1].save("data/K%s_standard.graph" % n)
    #fd = open("data/K%s_standard.guards" % n, "w")
    #fd.write(str(k))
    #fd.close()
        
main2()