#ifndef SINGLYLINKLIST_H
#define SINGLYLINKLIST_H

#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/manager.h"
#include "node.h"

// Singly Linked List class
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

    int preSt; // 1: addNodeAtHead, 2: addNodeAtTail, 3: addNodeAtPosition, 
                // 4: deleteNodeAtHead, 5: deleteNodeAtTail, 6: deleteNodeAtPosition

    std::string preDt;

    // Helper function to generate a position for visualization
    Vector2 generatePosition(int index);

    // Helper function to animate node movement
    void animateListMovement(ListNode* head, int start);
    void animateConnection(Vector2 start, Vector2 end, ListNode *node, Color color, float thickness);
    void appear(ListNode* node);
    void disappear(ListNode* node);
    
    // Button positions and sizes
    Rectangle addPosButton;
    Rectangle addHeadButton;
    Rectangle addTailButton;
    Rectangle deleteButton;
    Rectangle createRandomButton;
    Rectangle clearButton;
    // vector <Kind, pair <int, int>> buttonPos; 

    vector <button> toolButton;

    // Input fields
    char inputBuffer[10];
    bool inputActive;
    int inputType; // 0: None, 1: Add Head, 2: Add Tail, 3: Delete, 4: Create Random List

    bool isPseudocodeVisible;
    Rectangle togglePseudocodeButton; 
    const char** currentPseudocode; 
    int currentPseudocodeLineCount; 
    int highlightedLine;

    void updatePseudocodeOn();

    // Pseudocode for operations
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

    // Helper function to draw pseudocode for a specific operation
    void drawPseudocode(const char** pseudocode, int lineCount, int highlightedLine);

public:
    SinglyLinkedList();

    // Initialize the singly linked list
    void init();

    // Add a node at the head
    void addNodeAtHead(std::string data);

    // Add a node at the tail
    void addNodeAtTail(std::string data);

    // Add a node at the given position
    void addNodeAtPosition(int position, std::string data);

    void deleteNodeAtHead(); // Delete the head node
    void deleteNodeAtTail(); // Delete the tail node

    // Delete a node with the given data
    void deleteNode(int position);

    // Create a random singly linked list with n nodes
    void createRandomList(int n);

    // Clear the entire linked list
    void clear();

    void solveRemote();

    // Draw the singly linked list
    void draw();

    // Draw the operation buttons
    void drawButtons();

    // Handle button events
    void handleEvents();

    // Destructor to free memory
    ~SinglyLinkedList();
};

#endif