#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
#include "../include/node.h"
#include "../include/manager.h"
#include "../include/SinglyLinkedList.h"

// ListNode constructor
SinglyLinkedList::ListNode::ListNode(Node* n) : node(n), next(nullptr) {}

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
    int col = index % 11;
    int row = index / 11;
    if (row % 2 == 1) col = 10 - col; // Reverse the column for every alternate row
    return { 300.0f + col * 100.0f, 200.0f + row * 100.0f }; // Horizontal line with constant distance
}

// Draw the singly linked list
void SinglyLinkedList::draw() {
    ListNode* current = head;
    while (current != nullptr) {
        current->node->draw();
        if (current->next != nullptr) {
            DrawConnection(current->node->position, current->next->node->position, 1, BLACK, 4.0f, current->node->radius, current->next->node->radius);
        }
        current = current->next;
    }
}

// Helper function to animate node movement
void SinglyLinkedList::animateListMovement(ListNode* head, int start) {
    float duration = deltaTime; // Adjust duration based on operation speed
    float elapsedTime = 0.0f;
    while (elapsedTime < duration) {
        float t = elapsedTime / duration;
        int idx = start;
        ListNode* current = head;
        while (current != nullptr) {
            Vector2 startPosition = current->node->position;
            Vector2 endPosition = generatePosition(idx++);
            current->node->position = {
                startPosition.x + t * (endPosition.x - startPosition.x),
                startPosition.y + t * (endPosition.y - startPosition.y)
            };
            current = current->next;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        tool.draw();
        tool.UpdatePressOn();
        draw();
        drawButtons();
        EndDrawing();
        elapsedTime += GetFrameTime();
    }
}

void SinglyLinkedList::appear(ListNode* node) {
    float duration = deltaTime;
    float elapsedTime = 0.0f;
    float initialRadius = 0.0f; float finalRadius = node->node->radius;
    float initialFontSize = 0.0f; float finalFontSize = node->node->fontSize;
    
    while (elapsedTime < duration) {
        float t = elapsedTime / duration;
        float currentRadius = initialRadius + t * (finalRadius - initialRadius);
        float currentFontSize = initialFontSize + t * (finalFontSize - initialFontSize);
        node->node->radius = currentRadius;
        node->node->fontSize = currentFontSize;

        BeginDrawing();
        tool.draw();
        tool.UpdatePressOn();
        node->node->draw();
        draw();
        drawButtons();
        EndDrawing();

        elapsedTime += GetFrameTime();
    }

    node->node->radius = finalRadius;
    node->node->fontSize = finalFontSize;
}

void SinglyLinkedList::disappear(ListNode* node) {
    float duration = deltaTime;
    float elapsedTime = 0.0f;
    float initialRadius = node->node->radius;
    float initialFontSize = node->node->fontSize;

    while (elapsedTime < duration) {
        float t = elapsedTime / duration;
        float currentRadius = initialRadius - t * initialRadius;
        float currentFontSize = initialFontSize - t * initialFontSize;
        node->node->radius = currentRadius;
        node->node->fontSize = currentFontSize;

        BeginDrawing();
        tool.draw();
        tool.UpdatePressOn();
        node->node->draw();
        draw();
        drawButtons();
        EndDrawing();

        elapsedTime += GetFrameTime();
    }
}

// Add a node at the head
void SinglyLinkedList::addNodeAtHead(std::string data) {
    if (size > 0) animateListMovement(head, 1);
    Vector2 position = generatePosition(0);
    Node* newNode = new Node(data, position, ORANGE, radius, customFont, fontSize);
    ListNode* newListNode = new ListNode(newNode);

    appear(newListNode);
    newListNode->node->color = BLACK;

    newListNode->next = head;
    head = newListNode;
    if (tail == nullptr) {
        tail = head;
    }
    size++;

    // Update positions with animation
    ListNode* current = head;
    int index = 0;
    while (current != nullptr) {
        Vector2 newPosition = generatePosition(index++);
        current->node->position = newPosition;
        current = current->next;
    }
}

// Add a node at the tail
void SinglyLinkedList::addNodeAtTail(std::string data) {
    Vector2 position = generatePosition(size);
    Node* newNode = new Node(data, position, ORANGE, radius, customFont, fontSize);
    ListNode* newListNode = new ListNode(newNode);
    if (tail == nullptr) {
        head = tail = newListNode;
    } else {
        tail->next = newListNode;
        tail = newListNode;
    }
    size++;

    // Update positions with animation
    ListNode* current = head;
    int index = 0;
    while (current != nullptr) {
        Vector2 newPosition = generatePosition(index++);
        current->node->position = newPosition;
        current = current->next;
    }

    appear(tail);
    tail->node->color = BLACK;
}

// Delete a node with the given data
void SinglyLinkedList::deleteNode(std::string data) {
    ListNode* current = head;
    ListNode* previous = nullptr;
    while (current != nullptr && current->node->data != data) {
        previous = current;
        current = current->next;
    }
    if (current == nullptr) return; // Node not found

    current->node->color = ORANGE;
    disappear(current);
    
    if (previous == nullptr) {
        head = current->next;
    } else {
        previous->next = current->next;
    }
    if (current == tail) {
        tail = previous;
    }
    
    delete current->node;
    delete current;
    size--;

    if (size > 0) animateListMovement(head, 0);

    // Update positions with animation
    current = head;
    int index = 0;
    while (current != nullptr) {
        Vector2 newPosition = generatePosition(index++);
        current->node->position = newPosition;
        current = current->next;
    }
}

// Create a random singly linked list with n nodes
void SinglyLinkedList::createRandomList(int n) {
    clear();
    for (int i = 0; i < n; ++i) {
        int data = rand() % 100; // Random data between 0 and 99
        addNodeAtTail(std::to_string(data));
    }
}

// Clear the entire linked list
void SinglyLinkedList::clear() {
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* next = current->next;
        delete current->node;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size = 0;
}

// Draw the operation buttons
void SinglyLinkedList::drawButtons() {
    DrawRectangleRec(addHeadButton, LIGHTGRAY);
    DrawTextEx(customFont, "Add Node at Head", { addHeadButton.x + 10, addHeadButton.y + 15 }, 20, 2, BLACK);

    DrawRectangleRec(addTailButton, LIGHTGRAY);
    DrawTextEx(customFont, "Add Node at Tail", { addTailButton.x + 10, addTailButton.y + 15 }, 20, 2, BLACK);

    DrawRectangleRec(deleteButton, LIGHTGRAY);
    DrawTextEx(customFont, "Delete Node", { deleteButton.x + 10, deleteButton.y + 15 }, 20, 2, BLACK);

    DrawRectangleRec(createRandomButton, LIGHTGRAY);
    DrawTextEx(customFont, "Create Random List", { createRandomButton.x + 10, createRandomButton.y + 15 }, 20, 2, BLACK);

    DrawRectangleRec(clearButton, LIGHTGRAY);
    DrawTextEx(customFont, "Clear List", { clearButton.x + 10, clearButton.y + 15 }, 20, 2, BLACK);

    // Draw input box if active
    if (inputActive) {
        DrawRectangle(300, 650, 200, 50, LIGHTGRAY);
        DrawTextEx(customFont, inputBuffer, { 310, 665 }, 20, 2, BLACK);
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
            std::string value = inputBuffer;
            if (inputType == 1) {
                addNodeAtHead(value);
            } else if (inputType == 2) {
                addNodeAtTail(value);
            } else if (inputType == 3) {
                deleteNode(value);
            } else if (inputType == 4) {
                createRandomList(std::stoi(value));
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