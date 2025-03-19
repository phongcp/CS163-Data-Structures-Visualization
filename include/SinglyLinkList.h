#ifndef SINGLYLINKLIST_H
#define SINGLYLINKLIST_H

#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"

// Singly Linked List class
class SinglyLinkedList {
private:
    struct Node {
        int data;
        Node* next;
        Vector2 position; // Position for visualization

        Node(int data, Vector2 position);
    };

    Node* head;
    Node* tail;
    int size;

    // Helper function to generate a position for visualization
    Vector2 generatePosition(int index);

    // Button positions and sizes
    Rectangle addHeadButton;
    Rectangle addTailButton;
    Rectangle deleteButton;
    Rectangle createRandomButton;
    Rectangle clearButton;

    // Input fields
    char inputBuffer[10];
    bool inputActive;
    int inputType; // 0: None, 1: Add Head, 2: Add Tail, 3: Delete

public:
    SinglyLinkedList();

    // Initialize the singly linked list
    void init();

    // Add a node at the head
    void addNodeAtHead(int data);

    // Add a node at the tail
    void addNodeAtTail(int data);

    // Delete a node with the given data
    void deleteNode(int data);

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