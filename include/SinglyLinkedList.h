#ifndef SINGLYLINKLIST_H
#define SINGLYLINKLIST_H

#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/manager.h"
#include "../include/var.h"
#include "../include/node.h"

class SinglyLinkedList {
private:
    vector <Node> list;
    Node pnode;
    bool visb;

    Vector2 generatePosition(int index);

    void animateListMovement(vector <Node> &nodes, vector <Node> newNodes);
    void animateConnection(Vector2 start, Vector2 end, Color color, float thickness);
    void animateDisconnection(Vector2 start, Vector2 end, Color color, float thickness);
    void appear(Node &node);
    void disappear(Node &node);
    
    Rectangle loadFileButton;
    Rectangle addPosButton;
    Rectangle addHeadButton;
    Rectangle addTailButton;
    Rectangle deleteButton;
    Rectangle createRandomButton;
    Rectangle clearButton;
    vector <button> remote;

    char inputBuffer[10];
    bool inputActive;
    int inputType; // 0: None, 1: Add Head, 2: Add Tail, 3: Delete, 4: Create Random List

    bool isPseudocodeVisible;
    Rectangle togglePseudocodeButton; 
    const char** currentPseudocode; 
    int currentPseudocodeLineCount; 
    int highlightedLine;

    void updatePseudocodeOn();

    int idx;
    bool isRunning;
    struct State {
        int curline, curop, pos; // 0: None, 1: List Moving, 2: Appear, 3: Disappear, 4: Conection
        vector <Node> nodes;
    };
    vector <State> history;

    const char* addNodeAtHeadCode[6] = {
        "Vertex vtx = new Vertex(v)",
        "vtx.next = head",
        "head = vtx",
        "",
        "",
        ""
    };

    const char* addNodeAtTailCode[6] = {
        "Vertex vtx = new Vertex(v)",
        "tail.next = vtx",
        "tail = vtx",
        "",
        "",
        ""
    };

    const char* addNodeAtPositionCode[6] = {
        "Vertex pre=head, for (k=0;k<i-1;k++) pre=pre.next",
        "Vertex aft = pre.next, Vertex vtx = new Vertex(v)",
        "vtx.next = aft, pre.next = vtx",
        "",
        "",
        ""
    };

    const char* deleteNodeAtHeadCode[6] = {
        "if empty, do nothing",
        "tmp = head, head = head.next, delete tmp",
        "",
        "",
        "",
        ""
    };

    const char* deleteNodeAtTailCode[6] = {
        "if empty, do nothing",
        "Vertex pre = head, tmp = head.next",
        "while (pre.next.next != null) pre = pre.next",
        "pre.next = null, delete tmp, tail = pre",
        "",
        "",
    };

    const char* deleteNodeAtPositionCode[6] = {
        "if empty, do nothing",
        "Vertex pre=head, for (k=0;k<i-1;k++) pre=pre.next",
        "Vertex del = pre.next, aft = del.next",
        "pre.next = aft, delete del",
        "",
        ""
    };

    void drawPseudocode(const char** pseudocode, int lineCount, int highlightedLine);

public:
    void init();
    void addNodeAtHead(std::string data);
    void addNodeAtTail(std::string data);
    void addNodeAtPosition(int pos, std::string data);
    void deleteNodeAtHead(); 
    void deleteNodeAtTail();
    void deleteNode(int position);
    void createRandomList(int n);
    void loadFile();
    void clear();
    void checkPause();
    void goingSt(int nidx);
    void running();
    void draw();
    void handleRemote();
    void handleEvents();
};

#endif