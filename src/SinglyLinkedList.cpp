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
    addPosButton = { 20, 340, 200, 50 };
    addHeadButton = { 20, 400, 200, 50 };
    addTailButton = { 20, 460, 200, 50 };
    deleteButton = { 20, 520, 200, 50 };
    createRandomButton = { 20, 580, 200, 50 };
    clearButton = { 20, 640, 200, 50 };

    toolButton.resize(6);
    toolButton[0] = {{350,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/pre.png"),RAYWHITE,1};    
    toolButton[1] = {{toolButton[0].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/undo.png"),RAYWHITE,1};
    toolButton[2] = {{toolButton[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/play.png"),RAYWHITE,1};
    toolButton[3] = {{toolButton[2].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/redo.png"),RAYWHITE,1};
    toolButton[4] = {{toolButton[3].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/suff.png"),RAYWHITE,1};

    // Khởi tạo trạng thái bảng pseudocode
    isPseudocodeVisible = false;
    currentPseudocode = nullptr;
    currentPseudocodeLineCount = 6;
    highlightedLine = -1;

    // Vị trí và kích thước nút đóng/mở bảng pseudocode
    togglePseudocodeButton = { (float)screenWidth-40, 520, 40, 110 };
}

// Helper function to generate a position for visualization
Vector2 SinglyLinkedList::generatePosition(int index) {
    int col = index % 11;
    int row = index / 11;
    if (row % 2 == 1) col = 10 - col; // Reverse the column for every alternate row
    return { 300.0f + col * 100.0f, 200.0f + row * 100.0f }; // Horizontal line with constant distance
}

void SinglyLinkedList::updatePseudocodeOn() {
    Vector2 mousePosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePosition, togglePseudocodeButton)) {
            isPseudocodeVisible = !isPseudocodeVisible; 
        }
    }
}

void SinglyLinkedList::drawPseudocode(const char** pseudocode, int lineCount, int highlightedLine) {
    float startX = (float)screenWidth - 605;
    float startY = 730 - lineCount * 40;

    float boxWidth = 560;
    float boxHeight = 40;

    for (int i = 0; i < lineCount; i++) {
        Rectangle textBox = { startX, startY + i * boxHeight, boxWidth, boxHeight };

        Color backgroundColor = (i == highlightedLine) ? GREEN: LIGHTGRAY;
        DrawRectangleRec(textBox, backgroundColor);

        Color textColor = (i == highlightedLine) ? WHITE : BLACK;
        DrawTextEx(customFont, pseudocode[i], { textBox.x + 10, textBox.y + 10 }, 20, 2, textColor);
    }
}

// Draw the singly linked list
void SinglyLinkedList::draw() {
    ListNode* current = head;
    int index = 0;

    while (current != nullptr) {
        current->node->draw();

        // Draw connection between nodes
        if (current->next != nullptr) {
            DrawConnection(current->node->position, current->next->node->position, 1, BLACK, 4.0f, current->node->radius, current->next->node->radius);
        }

        current = current->next;
        index++;
    }

    // Draw "head/0" below the head node
    if (head != nullptr) {
        Vector2 headPosition = head->node->position;
        DrawTextEx(customFont, "head/0", { headPosition.x - 20, headPosition.y + 30 }, 20, 2, RED);
    }

    // Draw "tail/(size)" below the tail node
    if (size > 1 && tail != nullptr) {
        Vector2 tailPosition = tail->node->position;
        std::string tailText = "tail/" + std::to_string(size - 1);
        DrawTextEx(customFont, tailText.c_str(), { tailPosition.x - 20, tailPosition.y + 30 }, 20, 2, RED);
    }

    if (isPseudocodeVisible) {
        if (currentPseudocode != nullptr) {
            drawPseudocode(currentPseudocode, currentPseudocodeLineCount, highlightedLine);
        } else {
            Rectangle pseudocodeBox = { 
                (float)screenWidth - 605, 
                730.0f - (float)(currentPseudocodeLineCount * 40), 
                560.0f, 
                40.0f * (float)currentPseudocodeLineCount 
            };
            DrawRectangleRec(pseudocodeBox, LIGHTGRAY);
        }
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
        updatePseudocodeOn();
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
        updatePseudocodeOn();
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
        updatePseudocodeOn();
        node->node->draw();
        draw();
        drawButtons();
        EndDrawing();

        elapsedTime += GetFrameTime();
    }
}

void SinglyLinkedList::animateConnection(Vector2 p1, Vector2 p2, ListNode* node, Color color, float thickness) {
    float elapsedTime = 0.0f;
    float duration = deltaTime; // Adjust duration based on operation speed
    Vector2 start = GetCircleEdgePoint(p1, p2, radius);
    Vector2 end = GetCircleEdgePoint(p2, p1, radius);

    while (elapsedTime < duration) {
        float t = elapsedTime / duration; // Calculate the interpolation factor (0.0 to 1.0)

        Vector2 currentEnd = {
            start.x + t * (end.x - start.x),
            start.y + t * (end.y - start.y)
        };

        BeginDrawing();

        tool.draw();
        tool.UpdatePressOn();
        updatePseudocodeOn();
        node->node->draw(); // Draw the node at its current position

        DrawLineEx(start, currentEnd, thickness, color);
        float angle = atan2(end.y - start.y, end.x - start.x);
        float arrowSize = 15;

        Vector2 left = { 
            currentEnd.x - (float)(arrowSize * cos(angle - PI / 5)), 
            currentEnd.y - (float)(arrowSize * sin(angle - PI / 5)) 
        };
    
        Vector2 right = { 
            currentEnd.x - (float)(arrowSize * cos(angle + PI / 5)), 
            currentEnd.y - (float)(arrowSize * sin(angle + PI / 5)) 
        };

        DrawLineEx(currentEnd, left, thickness-1, color);
        DrawLineEx(currentEnd, right, thickness-1, color);
        
        draw();
        drawButtons();

        EndDrawing();

        elapsedTime += GetFrameTime(); // Increment elapsed time by the frame time
    }

    // Ensure the full connection is drawn at the end with full opacity
    BeginDrawing();
    DrawLineEx(start, end, thickness, color);
    EndDrawing();
}

// Add a node at the head
void SinglyLinkedList::addNodeAtHead(std::string data) {
    currentPseudocode = addNodeAtHeadCode;

    highlightedLine = 0; // Highlight the first line of pseudocode
    
    if (size > 0) {
        animateListMovement(head, 1); // move all list nodes to the right
    } 

    Vector2 position = generatePosition(0);
    Node* newNode = new Node(data, position, ORANGE, radius, customFont, fontSize);
    ListNode* newListNode = new ListNode(newNode);

  
    appear(newListNode); // Animate the appearance of the new node

    highlightedLine = 1; // Highlight the second line of pseudocode

    newListNode->node->color = BLACK;
    newListNode->node->position = generatePosition(0);
    if (size > 0) {
        // Animate the connection to the next node
        animateConnection(generatePosition(0), generatePosition(1), newListNode, ORANGE, 4.0f);   
    }

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

    highlightedLine = 2; // Highlight the third line of pseudocode
}

// Add a node at the tail
void SinglyLinkedList::addNodeAtTail(std::string data) {
    currentPseudocode = addNodeAtTailCode;
    highlightedLine = 0; // Highlight the first line of pseudocode

    Vector2 position = generatePosition(size);
    Node* newNode = new Node(data, position, ORANGE, radius, customFont, fontSize);
    ListNode* newListNode = new ListNode(newNode);

    appear(newListNode);

    highlightedLine = 1; // Highlight the second line of pseudocode
    if (size > 0) {
        animateConnection(generatePosition(size - 1), generatePosition(size), newListNode, ORANGE, 4.0f);
    }

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

    tail->node->color = BLACK;

    highlightedLine = 2; // Highlight the third line of pseudocode
}

// Add a node at the given position
void SinglyLinkedList::addNodeAtPosition(int position, std::string data) {
    if (position < 0 || position > size) return; // Invalid position

    if (position == 0) {
        addNodeAtHead(data);
        return;
    } else if (position == size) {
        addNodeAtTail(data);
        return;
    }

    currentPseudocode = addNodeAtPositionCode;
    highlightedLine = 0; // Highlight the first line of pseudocode

    Vector2 newPosition = generatePosition(position);
    Node* newNode = new Node(data, newPosition, ORANGE, radius, customFont, fontSize);
    ListNode* newListNode = new ListNode(newNode);

    ListNode* current = head;
    for (int i = 0; i < position - 1; ++i) {
        float duration = deltaTime;
        float elapsedTime = 0.0f;
        while (elapsedTime < duration) {
            elapsedTime += GetFrameTime(); // Increment elapsed time by the frame time
            BeginDrawing();
            tool.draw();
            tool.UpdatePressOn();
            updatePseudocodeOn();
            current->node->color = ORANGE; // Highlight the current node
            draw();
            current->node->color = BLACK; // Highlight the current node
            drawButtons();
            EndDrawing();
        }
        current = current->next;
    }

    float duration = deltaTime;
    float elapsedTime = 0.0f;
    while (elapsedTime < duration) {
        elapsedTime += GetFrameTime(); // Increment elapsed time by the frame time
        BeginDrawing();
        tool.draw();
        tool.UpdatePressOn();
        updatePseudocodeOn();
        current->node->color = ORANGE; // Highlight the current node
        draw();
        current->node->color = BLACK; // Highlight the current node
        drawButtons();
        EndDrawing();
    }

    highlightedLine = 1; // Highlight the second line of pseudocode
    animateListMovement(current->next, position + 1);

    newListNode->next = current->next;
    current->next = newListNode;
    size++;

    // Update positions with animation
    ListNode* node = head;
    int index = 0;
    while (node != nullptr) {
        Vector2 updatedPosition = generatePosition(index++);
        node->node->position = updatedPosition;
        node = node->next;
    }

    appear(newListNode);
    newListNode->node->color = BLACK;

    highlightedLine = 2; // Highlight the third line of pseudocode
}

void SinglyLinkedList::deleteNodeAtHead() {
    currentPseudocode = deleteNodeAtHeadCode;
    highlightedLine = 0; // Highlight the first line of pseudocode

    float duration = deltaTime;
    float elapsedTime = 0.0f;
    while (elapsedTime < duration) {
        elapsedTime += GetFrameTime(); // Increment elapsed time by the frame time
        BeginDrawing();
        tool.draw();
        tool.UpdatePressOn();
        updatePseudocodeOn();
        draw();
        drawButtons();
        EndDrawing();
    }

    if (head == nullptr) return; // List is empty

    highlightedLine = 1; // Highlight the second line of pseudocode
    head->node->color = ORANGE; // Highlight the head node
    disappear(head);

    ListNode* temp = head;
    head = head->next;
    delete temp->node;
    delete temp;
    size--;

    if (size > 0) animateListMovement(head, 0);
}

void SinglyLinkedList::deleteNodeAtTail() {
    currentPseudocode = deleteNodeAtTailCode;
    highlightedLine = 0; // Highlight the first line of pseudocode
    
    float duration = deltaTime;
    float elapsedTime = 0.0f;
    while (elapsedTime < duration) {
        elapsedTime += GetFrameTime(); // Increment elapsed time by the frame time
        BeginDrawing();
        tool.draw();
        tool.UpdatePressOn();
        updatePseudocodeOn();
        draw();
        drawButtons();
        EndDrawing();
    }

    if (tail == nullptr) return; // List is empty

    highlightedLine = 1; // Highlight the second line of pseudocode

    duration = deltaTime;
    elapsedTime = 0.0f;
    while (elapsedTime < duration) {
        elapsedTime += GetFrameTime(); // Increment elapsed time by the frame time
        BeginDrawing();
        tool.draw();
        tool.UpdatePressOn();
        updatePseudocodeOn();
        draw();
        drawButtons();
        EndDrawing();
    }

    ListNode* current = head;
    ListNode* previous = nullptr;

    highlightedLine = 2;

    while (current->next != nullptr) {
        float duration = deltaTime;
        float elapsedTime = 0.0f;
        while (elapsedTime < duration) {
            elapsedTime += GetFrameTime();
            BeginDrawing();
            tool.draw();
            tool.UpdatePressOn();
            updatePseudocodeOn();
            current->node->color = ORANGE; // Highlight the current node
            draw();
            current->node->color = BLACK; // Highlight the current node
            drawButtons();
            EndDrawing();
        }

        previous = current;
        current = current->next;
    }

    highlightedLine = 3; 
    current->node->color = ORANGE; // Highlight the tail node
    disappear(current);

    if (previous == nullptr) {
        head = tail = nullptr; // List becomes empty
    } else {
        previous->next = nullptr;
        tail = previous;
    }
    delete current->node;
    delete current;
    size--;
}

// Delete a node with the given data
void SinglyLinkedList::deleteNode(int position) {
    if (position < 0 || position >= size) return; // Invalid position
    
    if (position == 0) {
        deleteNodeAtHead();
        return;
    } else if (position == size - 1) {
        deleteNodeAtTail();
        return;
    }

    currentPseudocode = deleteNodeAtPositionCode;
    highlightedLine = 0; // Highlight the first line of pseudocode

    float duration = deltaTime;
    float elapsedTime = 0.0f;
    while (elapsedTime < duration) {
        elapsedTime += GetFrameTime(); // Increment elapsed time by the frame time
        BeginDrawing();
        tool.draw();
        tool.UpdatePressOn();
        updatePseudocodeOn();
        draw();
        drawButtons();
        EndDrawing();
    }

    highlightedLine = 1; // Highlight the second line of pseudocode

    ListNode* current = head;
    ListNode* previous = nullptr;

    for (int i = 0; i < position; ++i) {
        float duration = deltaTime;
        float elapsedTime = 0.0f;
        while (elapsedTime < duration) {
            elapsedTime += GetFrameTime();
            BeginDrawing();
            tool.draw();
            tool.UpdatePressOn();
            updatePseudocodeOn();
            current->node->color = ORANGE; // Highlight the current node
            draw();
            current->node->color = BLACK; // Highlight the current node
            drawButtons();
            EndDrawing();
        }
        
        previous = current;
        current = current->next;
    }

    current->node->color = ORANGE;
    highlightedLine = 2; // Highlight the third line of pseudocode

    duration = deltaTime;
    elapsedTime = 0.0f;
    while (elapsedTime < duration) {
        elapsedTime += GetFrameTime(); // Increment elapsed time by the frame time
        BeginDrawing();
        tool.draw();
        tool.UpdatePressOn();
        updatePseudocodeOn();
        draw();
        drawButtons();
        EndDrawing();
    }

    highlightedLine = 3; // Highlight the fourth line of pseudocode
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
    currentPseudocode = nullptr;
    highlightedLine = -1; // Reset highlighted line

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
    DrawRectangleRec(addPosButton, LIGHTGRAY);
    DrawTextEx(customFont, "Add Node at Point", { addPosButton.x + 10, addPosButton.y + 15 }, 20, 2, BLACK);

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

    DrawRectangleRec(togglePseudocodeButton, LIGHTGRAY);
    if (isPseudocodeVisible) {
        DrawRectangleLinesEx(togglePseudocodeButton, 2, GREEN);
        DrawTextEx(customFont, ">", { togglePseudocodeButton.x + 8, togglePseudocodeButton.y + 35 }, 40, 2, GREEN);
    } else {
        DrawRectangleLinesEx(togglePseudocodeButton, 2, BLACK);
        DrawTextEx(customFont, "<", { togglePseudocodeButton.x + 8, togglePseudocodeButton.y + 35 }, 40, 2, BLACK);
    }
    
    // Draw input box if active
    if (inputActive) {
        float dy;
        if (inputType == 1) dy = 400;
        else if (inputType == 2) dy = 460;
        else if (inputType == 3) dy = 520;
        else if (inputType == 4) dy = 580;
        else if (inputType == 5) dy = 340;
        DrawRectangle(230, dy, 100, 50, LIGHTGRAY);
        DrawTextEx(customFont, inputBuffer, { 240, dy+15 }, 20, 2, BLACK);
    }

    Vector2 Mouse = GetMousePosition();
    for (button v : toolButton) {
        if (v.CheckMouse(Mouse,10)) v.col = 1;
        else v.col = 0;
        v.DrawBasic(0.6);
    }
}

// Handle button events
void SinglyLinkedList::handleEvents() {
    Vector2 mousePosition = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePosition, togglePseudocodeButton)) {
            isPseudocodeVisible = !isPseudocodeVisible;
        }

        if (CheckCollisionPointRec(mousePosition, addPosButton)) {
            inputActive = true;
            inputType = 5;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

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
            if (((key >= 48) && (key <= 57) && strlen(inputBuffer) < 9) || (key == 32 && inputType == 5)) {
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
                int tmp = 0;
                for (char x: value) tmp = tmp * 10 + x - '0';
                deleteNode(tmp);
            } else if (inputType == 4) {
                createRandomList(std::stoi(value));
            } else if (inputType == 5) {
                int pos = 0, tmp = 0, j = 0;
                while (j < value.size() && value[j] != ' ') pos = pos * 10 + value[j++] - '0';
                j++; 
                if (j >= value.size()) tmp = rand() % 100;
                else for (int i = j; i < value.size(); i++) tmp = tmp * 10 + value[i] - '0';
                addNodeAtPosition(pos, std::to_string(tmp));
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