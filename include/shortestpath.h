#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "../include/var.h"
#include "../include/manager.h"
#include "../include/node.h"
#include <bits/stdc++.h>
#include <raylib.h>

class ShortestPath {
private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<Edge> spEdges;
    vector <int> dist;
    vector <int> curdist;

    float edgeLength = 200.0f;
    float desiredDistance = 400.0f;

    Rectangle loadFileButton;
    Rectangle createButton;
    Rectangle dijkstraButton;
    Rectangle clearButton;
    vector <button> remote;

    char inputBuffer[10];
    bool inputActive;
    int inputType;

    bool isPseudocodeVisible;
    Rectangle togglePseudocodeButton; 
    const char** currentPseudocode; 
    int currentPseudocodeLineCount; 
    int highlightedLine;

    int idx;
    bool isRunning;
    const char* dijkstraCode[6] = {
        "SinitSSSP, pre-populate PQ",
        "while !PQ.empty() // PQ is a Priority Queue",
        "   for each neighbor v of u = PQ.front(), PQ.pop()",
        "       relax(u, v, w(u, v)) + update PQ",
        "",
        ""
    };

public:
    void init();
    void handleEvents();
    void handleRemote();
    void updatePseudocodeOn();
    void drawPseudocode(const char** pseudocode, int lineCount, int highlightedLine);
    void draw();
    void animateConnection(Vector2 p1, Vector2 p2, Color color, float thickness=4.0f);
    void addNode(Vector2 position);
    void create(int n, int m);
    void loadFile();
    void dijkstra(int s);
    void findSP(int s);
    void checkPause();
    void goingSt(int nidx);
    void running();
    void clear();
    void updateEdgeLengths();
    void updatePhysics(float stiffness=0.03f, float damping=0.8f);
    void handleMouseInput();
};

#endif
