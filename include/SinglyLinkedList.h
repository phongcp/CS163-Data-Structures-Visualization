#ifndef SINGLYLINKLIST_H
#define SINGLYLINKLIST_H

#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/manager.h"
#include "../include/var.h"
#include "../include/node.h"

class SinglyLinkedList {
private:
    struct ListNode {
        Node* node;
        ListNode* next;

        ListNode(Node* n);
    };

    ListNode* head;
    ListNode* tail;
    int size;

    Vector2 generatePosition(int index);

    void animateListMovement(ListNode* head, int start);
    void animateConnection(Vector2 start, Vector2 end, ListNode *node, Color color, float thickness);
    void appear(ListNode* node);
    void disappear(ListNode* node);
    
    Rectangle addPosButton;
    Rectangle addHeadButton;
    Rectangle addTailButton;
    Rectangle deleteButton;
    Rectangle createRandomButton;
    Rectangle clearButton;
    vector <button> toolButton;

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
    struct state {
        int curline;
        vector <Node> nodes;
    };
    int curOp;
    vector <state> history;

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
    SinglyLinkedList();
    void init();
    void addNodeAtHead(std::string data);
    void addNodeAtTail(std::string data);
    void addNodeAtPosition(int position, std::string data);
    void deleteNodeAtHead(); 
    void deleteNodeAtTail();
    void deleteNode(int position);
    void createRandomList(int n);
    void clear();
    void solveRemote();
    void draw();
    void drawButtons();
    void handleEvents();
    ~SinglyLinkedList();
};

#endif