#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
#include "../include/SinglyLinkList.h"

// Node constructor
SinglyLinkedList::Node::Node(int data, Vector2 position) : data(data), next(nullptr), position(position) {}

// SinglyLinkedList constructor
SinglyLinkedList::SinglyLinkedList() : head(nullptr), tail(nullptr), size(0), inputActive(false), inputType(0) {
    memset(inputBuffer, 0, sizeof(inputBuffer));
}

// Initialize the singly linked list
void SinglyLinkedList::init() {
    // Initialize button positions and sizes
    addHeadButton = { 50, 350, 200, 50 };
    addTailButton = { 50, 410, 200, 50 };
    deleteButton = { 50, 470, 200, 50 };
    createRandomButton = { 50, 530, 200, 50 };
    clearButton = { 50, 590, 200, 50 };
}

// Helper function to generate a position for visualization
Vector2 SinglyLinkedList::generatePosition(int index) {
    int col = index % 10;
    int row = index / 10;
    return { 300.0f + col * 100.0f, 200.0f + row * 100.0f }; // Horizontal line with constant distance
}

// Add a node at the head
void SinglyLinkedList::addNodeAtHead(int data) {
    Vector2 position = generatePosition(0);
    Node* newNode = new Node(data, position);
    newNode->next = head;
    head = newNode;
    if (tail == nullptr) {
        tail = head;
    }
    size++;

    // Update positions
    Node* current = head;
    int index = 0;
    while (current != nullptr) {
        current->position = generatePosition(index++);
        current = current->next;
    }
}

// Add a node at the tail
void SinglyLinkedList::addNodeAtTail(int data) {
    Vector2 position = generatePosition(size);
    Node* newNode = new Node(data, position);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;

    // Update positions
    Node* current = head;
    int index = 0;
    while (current != nullptr) {
        current->position = generatePosition(index++);
        current = current->next;
    }
}

// Delete a node with the given data
void SinglyLinkedList::deleteNode(int data) {
    Node* current = head;
    Node* previous = nullptr;
    while (current != nullptr && current->data != data) {
        previous = current;
        current = current->next;
    }
    if (current == nullptr) return; // Node not found
    if (previous == nullptr) {
        head = current->next;
    } else {
        previous->next = current->next;
    }
    if (current == tail) {
        tail = previous;
    }
    delete current;
    size--;

    // Update positions
    current = head;
    int index = 0;
    while (current != nullptr) {
        current->position = generatePosition(index++);
        current = current->next;
    }
}

// Create a random singly linked list with n nodes
void SinglyLinkedList::createRandomList(int n) {
    clear();
    for (int i = 0; i < n; ++i) {
        int data = rand() % 100; // Random data between 0 and 99
        addNodeAtTail(data);
    }
}

// Clear the entire linked list
void SinglyLinkedList::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size = 0;
}

// Draw the singly linked list
void SinglyLinkedList::draw() {
    Node* current = head;
    while (current != nullptr) {
        DrawCircleV(current->position, 30, BLUE); // Increased size of the node
        DrawText(TextFormat("%d", current->data), current->position.x - 15, current->position.y - 15, 30, WHITE); // Increased font size
        if (current->next != nullptr) {
            DrawLineEx(current->position, current->next->position, 5, BLACK); // Increased thickness of the line
        }
        current = current->next;
    }
}

// Draw the operation buttons
void SinglyLinkedList::drawButtons() {
    DrawRectangleRec(addHeadButton, LIGHTGRAY);
    DrawText("Add Node at Head", addHeadButton.x + 10, addHeadButton.y + 15, 20, BLACK);

    DrawRectangleRec(addTailButton, LIGHTGRAY);
    DrawText("Add Node at Tail", addTailButton.x + 10, addTailButton.y + 15, 20, BLACK);

    DrawRectangleRec(deleteButton, LIGHTGRAY);
    DrawText("Delete Node", deleteButton.x + 10, deleteButton.y + 15, 20, BLACK);

    DrawRectangleRec(createRandomButton, LIGHTGRAY);
    DrawText("Create Random List", createRandomButton.x + 10, createRandomButton.y + 15, 20, BLACK);

    DrawRectangleRec(clearButton, LIGHTGRAY);
    DrawText("Clear List", clearButton.x + 10, clearButton.y + 15, 20, BLACK);

    // Draw input box if active
    if (inputActive) {
        DrawRectangle(300, 650, 200, 50, LIGHTGRAY);
        DrawText(inputBuffer, 310, 665, 20, BLACK);
    }
}

// Handle button events
void SinglyLinkedList::handleEvents() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePosition = GetMousePosition();

        // Add node at head button
        if (CheckCollisionPointRec(mousePosition, addHeadButton)) {
            inputActive = true;
            inputType = 1;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        // Add node at tail button
        if (CheckCollisionPointRec(mousePosition, addTailButton)) {
            inputActive = true;
            inputType = 2;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        // Delete node button
        if (CheckCollisionPointRec(mousePosition, deleteButton)) {
            inputActive = true;
            inputType = 3;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        // Create random list button
        if (CheckCollisionPointRec(mousePosition, createRandomButton)) {
            inputActive = true;
            inputType = 4;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        // Clear list button
        if (CheckCollisionPointRec(mousePosition, clearButton)) {
            clear();
        }
    }

    // Handle keyboard input for active input
    if (inputActive) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 48) && (key <= 57) && strlen(inputBuffer) < 9) {
                int len = strlen(inputBuffer);
                inputBuffer[len] = (char)key;
                inputBuffer[len + 1] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = strlen(inputBuffer);
            if (len > 0) inputBuffer[len - 1] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER)) {
            int value = atoi(inputBuffer);
            if (inputType == 1) {
                addNodeAtHead(value);
            } else if (inputType == 2) {
                addNodeAtTail(value);
            } else if (inputType == 3) {
                deleteNode(value);
            } else if (inputType == 4) {
                createRandomList(value);
            }
            inputActive = false;
            inputType = 0;
        }
    }
}

// Destructor to free memory
SinglyLinkedList::~SinglyLinkedList() {
    clear();
}