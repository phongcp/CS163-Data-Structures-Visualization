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

    // Helper function to generate a position for visualization
    Vector2 generatePosition(int index);

    // Helper function to animate node movement
    void animateListMovement(ListNode* head, int start);
    void appear(ListNode* node);
    void disappear(ListNode* node);

    // Button positions and sizes
    Rectangle addHeadButton;
    Rectangle addTailButton;
    Rectangle deleteButton;
    Rectangle createRandomButton;
    Rectangle clearButton;

    // Input fields
    char inputBuffer[10];
    bool inputActive;
    int inputType; // 0: None, 1: Add Head, 2: Add Tail, 3: Delete, 4: Create Random List

public:
    SinglyLinkedList();

    // Initialize the singly linked list
    void init();

    // Add a node at the head
    void addNodeAtHead(std::string data);

    // Add a node at the tail
    void addNodeAtTail(std::string data);

    // Delete a node with the given data
    void deleteNode(std::string data);

    // Create a random singly linked list with n nodes
    void createRandomList(int n);

    // Clear the entire linked list
    void clear();

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