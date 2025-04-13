#ifndef MST_H
#define MST_H

#include "../include/var.h"
#include "../include/manager.h"
#include "../include/node.h"
#include <bits/stdc++.h>
#include <raylib.h>

class MST {
private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<Edge> mstEdges;

    float edgeLength = 200.0f;
    float desiredDistance = 400.0f;

    Rectangle loadFileButton;
    Rectangle createButton;
    Rectangle mstButton;
    Rectangle clearButton;
    vector <button> remote;

    char inputBuffer[10];
    bool inputActive;

    bool isPseudocodeVisible;
    Rectangle togglePseudocodeButton; 
    const char** currentPseudocode; 
    int currentPseudocodeLineCount; 
    int highlightedLine;

    int idx;
    bool isRunning;
    const char* kruskalCode[6] = {
        "Sort E edges by increasing weight, T = {}",
        "for (i = 0; i < edgeList.length; i++)",
        "   if adding e = edgelist[i] does not form a cycle",
        "       add e to T",
        "   else ignore e",
        "MST = T"
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
    void kruskal();
    void findMST();
    void checkPause();
    void goingSt(int nidx);
    void running();
    void clear();
    void updateEdgeLengths();
    void updatePhysics(float stiffness=0.03f, float damping=0.8f);
    void handleMouseInput();
};

#endif // MST_H
