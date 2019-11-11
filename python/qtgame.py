import sys
import os
import math
import re
from PyQt5.QtCore import (QLineF, QPointF, QRectF, QSizeF, Qt)
from PyQt5.QtGui import (QPen, QBrush, QColor, QPolygonF, QPainter, QTransform)
from PyQt5.QtWidgets import *
from PyQt5 import uic
from eternaldom import *
    
   
class Arrow (QGraphicsItem):
    def __init__(self, x1, y1, x2, y2):
        QGraphicsItem.__init__(self)
        self.setZValue(-1)
        self.x1 = x1
        self.x2 = x2
        self.y1 = y1
        self.y2 = y2
        self.arrowSize = 12
        self.penWidth = 3

    def paint(self, painter, option, widget):
        line = QLineF(self.x2, self.y2, self.x1, self.y1)
        line.setLength(line.length() - 20)
        line = QLineF(line.p2(), line.p1())
        line.setLength(line.length() - 20)
        
        angle = math.acos(line.dx() / line.length())
        if line.dy() >= 0:
            angle = 2 * math.pi - angle

        painter.setPen(QPen(Qt.black, self.penWidth, Qt.SolidLine, Qt.RoundCap, Qt.RoundJoin))
        painter.setBrush(Qt.black)
        
        painter.drawLine(line)
    
        destArrowP1 = line.p2() + QPointF(math.sin(angle - math.pi / 3) * self.arrowSize,
                                                math.cos(angle - math.pi / 3) * self.arrowSize)
        destArrowP2 = line.p2() + QPointF(math.sin(angle - math.pi + math.pi / 3) * self.arrowSize,
                                                math.cos(angle - math.pi + math.pi / 3) * self.arrowSize)

        polygon = QPolygonF()
        polygon.append(line.p2())
        polygon.append(destArrowP1)
        polygon.append(destArrowP2)
        painter.drawPolygon(polygon)

    def boundingRect(self):
        extra = (self.penWidth + self.arrowSize) / 2.0
        return QRectF(QPointF(self.x1, self.y1), QSizeF(self.x2 - self.x1, self.y2 - self.y1)).normalized().adjusted(-extra, -extra, extra, extra)
        
        
class Scene(QGraphicsScene):
    def __init__(self): 
        QGraphicsScene.__init__(self)
    
    def generateGraphName(self, type, n, m):
        if type == 'grid':
            return "G%s,%s" % (n, m)
        elif type == 'torus':
            return "T%s,%s" % (n, m)
        elif type == 'biclique':
            return "K%s,%s" % (n, m)
        elif type == "clique":
            return "K%s" % n
        else:
            return None
    
    
    def generate(self, type, n, m, rules):
        self.clear()
        self.type = type
        self.n = n
        self.m = m
        graphName = self.generateGraphName(type, n, m)
        self.graph = loadGraph("data/%s_%s.graph" % (graphName, rules))
        nb_guards = int(open("data/%s_%s.guards" % (graphName, rules)).readline())
        self.arena = computeArena(self.graph, nb_guards, rules)
        self.guards = None
        if type == 'grid' or type == 'torus':
            self.initGrid()
        elif type == 'biclique':
            self.initBiclique()
        elif type == 'clique':
            self.initClique()

        self.guards = self.arena.startingConf()
        self.updateGuards()
            
    def initGrid(self):
        self.table = {}
        n = self.n
        m = self.m
        graph = self.graph
        for i in range(n):
            for j in range(m):
                self.table[i*m+j] = self.addEllipse(30. + 100. * i, 30 + 100. * j, 40., 40.)

        for u in range(len(graph)):
            for v in graph.neighbors(u):
                i1, j1 = u // m, u % m
                i2, j2 = v // m, v % m
                if j1 == 0 and j2 == m-1:
                    self.addItem(Arrow(50+i1*100, 50+j1*100, 50+i2*100, -10))
                    self.addItem(Arrow(50+i1*100, 110+j2*100, 50+i2*100, 50+j2*100))
                elif j1 == m-1 and j2 == 0:
                    self.addItem(Arrow(50+i1*100, 50+j1*100, 50+i2*100, 110+j1*100))
                    self.addItem(Arrow(50+i1*100, -10, 50+i2*100, 50+j2*100))
                elif i1 == 0 and i2 == n-1:
                    self.addItem(Arrow(50+i1*100, 50+j1*100, -10, 50+j2*100))
                    self.addItem(Arrow(110+i2*100, 50+j1*100, 50+i2*100, 50+j2*100))
                elif i1 == n-1 and i2 == 0:
                    self.addItem(Arrow(50+i1*100, 50+j1*100, 110+i1*100, 50+j2*100))
                    self.addItem(Arrow(-10,       50+j1*100, 50+i2*100,  50+j2*100))
                else:
                    self.addItem(Arrow(50+i1*100, 50+j1*100, 50+i2*100,  50+j2*100))



    def initBiclique(self):
        n = self.n
        m = self.m
        graph = self.graph
        self.table = {}
        for i in range(n):
            self.table[i] = self.addEllipse(30. + 100. * i, 30., 40., 40.)
        for i in range(m):
            self.table[n+i] = self.addEllipse(30. + 100. * i, 230., 40., 40.)
        for u in range(len(graph)):
            for v in graph.neighbors(u):
                center1 = self.table[u].boundingRect().center()
                center2 = self.table[v].boundingRect().center()
                self.addItem(Arrow(center1.x(), center1.y(), center2.x(), center2.y()))

    def initClique(self):
        n = self.n
        graph = self.graph
        self.table = {}
        for i in range(n):
            self.table[i] = self.addEllipse(30. + 50. * i, 30., 40., 40.)
        l = graph.neighbors(0)
        l += [i - 13 for i in l]
        text = self.addText(str(sorted(l)))
        text.setX(100)
        text.setY(100)

    def updateGuards(self):
        if self.guards is None:
            return
    
        for i, item in self.table.items():
            if i in self.guards:
                item.setBrush(Qt.blue)
            else:
                item.setBrush(Qt.transparent)
        self.update()
        
        
    def mousePressEvent(self, event):
        position = None
        
        item = self.itemAt(event.scenePos(), QTransform())
        for i, item2 in self.table.items():
            if item is item2:
                position = i
                break
        
        if position is not None and position not in self.guards:
            self.guards = self.arena.answer(self.guards + [position])
            self.updateGuards()

class Window(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        layout = QVBoxLayout()
        layout2 = QHBoxLayout()
        self.cbType = QComboBox()
        self.cbDimensions = QComboBox()
        self.cbRules = QComboBox()
        self.butStart = QPushButton("Generate")
        self.gView = QGraphicsView()
        layout2.addWidget(self.cbType)
        layout2.addWidget(self.cbDimensions)
        layout2.addWidget(self.cbRules)
        layout2.addWidget(self.butStart)
        layout.addLayout(layout2)
        layout.addWidget(self.gView)
        self.setLayout(layout)
        self.resize(600, 600)
        
        
class View:
    def __init__(self):
        #self.win = uic.loadUi('interface.ui')
        self.win = Window()
        self.data = []
        self.scene = Scene()
        self.win.gView.setScene(self.scene)
        self.win.cbType.activated.connect(self.cbTypeActivated)
        self.win.cbDimensions.activated.connect(self.cbDimsActivated)
        self.win.butStart.clicked.connect(self.generate)
    
    def cbTypeActivated(self):
        type = self.win.cbType.currentText()
        dims = sorted(set(x[1] for x in self.data if x[0] == type))
        rules = sorted(set(x[2] for x in self.data if x[0] == type and x[1] == dims[0]))
        
        self.win.cbDimensions.clear()
        self.win.cbRules.clear()
        for d in dims:
            self.win.cbDimensions.addItem(d)
        for r in rules:
            self.win.cbRules.addItem(r)

    def cbDimsActivated(self):
        type = self.win.cbType.currentText()
        dim = self.win.cbDimensions.currentText()
        dims = sorted(set(x[1] for x in self.data if x[0] == type))
        rules = sorted(set(x[2] for x in self.data if x[0] == type and x[1] == dim))
        
        self.win.cbRules.clear()
        for r in rules:
            self.win.cbRules.addItem(r)
            
        
    def start(self):
        self.scanDataDir()
        
        types = sorted(set(x[0] for x in self.data))
        dims = sorted(set(x[1] for x in self.data if x[0] == types[0]))
        rules = sorted(set(x[2] for x in self.data if x[0] == types[0] and x[1] == dims[0]))
        
        for t in types:
            self.win.cbType.addItem(t)
        for d in dims:
            self.win.cbDimensions.addItem(d)
        for r in rules:
            self.win.cbRules.addItem(r)
        
        self.win.show()

    def scanDataDir(self):
        d = { 'K' : 'biclique', 'G' : 'grid', 'T' : 'torus'}
        regexp = re.compile(r"(.)(\d+,\d+)_(\w+).graph")
        regexp_clique = re.compile(r"K(\d+)_(\w+).graph")
        for file in os.listdir('data/'):
            match = re.match(regexp_clique, file)
            if match is not None:
                x = ("clique", match.group(1), match.group(2))
                self.data.append(x)
                continue

            match = re.match(regexp, file)
            if match is not None:
                x = (d[match.group(1)], match.group(2), match.group(3))
                self.data.append(x)
                
    def generate(self):
        type = self.win.cbType.currentText()
        dimensions = self.win.cbDimensions.currentText()
        l = [int(x) for x in dimensions.split(",")]
        n = 0
        m = 0
        if len(l) == 1:
            n = l[0]
        else:
            n, m = l
        rules = self.win.cbRules.currentText()
        self.scene.generate(type, n, m, rules)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    view = View()
    view.start()
    app.exec_()
