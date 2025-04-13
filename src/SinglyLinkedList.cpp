#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/tinyfiledialogs.h"
#include "../include/var.h"
#include "../include/ui.h"
#include "../include/node.h"
#include "../include/manager.h"
#include "../include/SinglyLinkedList.h"

void SinglyLinkedList::init() {
    loadFileButton = { 20, 280, 200, 50 };
    addPosButton = { 20, 340, 200, 50 };
    addHeadButton = { 20, 400, 200, 50 };
    addTailButton = { 20, 460, 200, 50 };
    deleteButton = { 20, 520, 200, 50 };
    createRandomButton = { 20, 580, 200, 50 };
    clearButton = { 20, 640, 200, 50 };

    remote.resize(6);
    remote[0] = {{350,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/pre.png"),RAYWHITE,1};    
    remote[1] = {{remote[0].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/undo.png"),RAYWHITE,1};
    remote[2] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/play.png"),RAYWHITE,1};
    remote[3] = {{remote[2].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/redo.png"),RAYWHITE,1};
    remote[4] = {{remote[3].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/suff.png"),RAYWHITE,1};

    isRunning = false;
    isPseudocodeVisible = false;
    currentPseudocode = nullptr;
    currentPseudocodeLineCount = 6;
    highlightedLine = -1;

    togglePseudocodeButton = { (float)screenWidth-40, 520, 40, 110 };
}

Vector2 SinglyLinkedList::generatePosition(int index) {
    int col = index % 11;
    int row = index / 11;
    if (row % 2 == 1) col = 10 - col; 
    return { 300.0f + col * 100.0f, 200.0f + row * 100.0f };
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

void SinglyLinkedList::draw() {
    DrawRectangleRec(loadFileButton, LIGHTGRAY);
    DrawTextEx(customFont, "Load File", { loadFileButton.x + 10, loadFileButton.y + 15 }, 20, 2, BLACK);

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

    Vector2 Mouse = GetMousePosition();
    for (button v : remote) {
        if (v.CheckMouse(Mouse,10)) v.col = 1;
        else v.col = 0;
        v.DrawBasic(0.6);
    }
    
    updatePseudocodeOn();
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

    if (list.size() == 0) return ;

    for (int i = 0; i < list.size(); i++) {
        list[i].draw();
    }

    if (list.size() > 1) {
        for (int i = 0; i < list.size() - 1; i++) {
            DrawConnection(list[i].position, list[i + 1].position, 1, BLACK, 4.0f, list[i].radius, list[i + 1].radius);
        }
    }

    if (list.size() > 0) {
        Vector2 headPosition = list[0].position;
        DrawTextEx(customFont, "head/0", { headPosition.x - 20, headPosition.y + 30 }, 20, 2, RED);
    }

    if (list.size() > 1) {
        Vector2 tailPosition = list[list.size() - 1].position;
        std::string tailText = "tail/" + std::to_string(list.size() - 1);
        DrawTextEx(customFont, tailText.c_str(), { tailPosition.x - 20, tailPosition.y + 30 }, 20, 2, RED);
    }

    if (visb) pnode.draw();
}

void SinglyLinkedList::animateListMovement(vector <Node> &nodes, vector <Node> newNodes) {
    float duration = deltaTime;
    float elapsedTime = 0.0f;
    while (elapsedTime < duration) {
        float t = elapsedTime / duration;
        for (int i = 0; i < nodes.size(); i++) {
            Vector2 startPosition = nodes[i].position;
            Vector2 endPosition = newNodes[i].position;
            nodes[i].position = {
                startPosition.x + t * (endPosition.x - startPosition.x),
                startPosition.y + t * (endPosition.y - startPosition.y)
            };
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        tool.draw();
        tool.UpdatePressOn();
        draw();
        EndDrawing();
        elapsedTime += GetFrameTime();
    }
}

void SinglyLinkedList::appear(Node &node) {
    float duration = deltaTime;
    float elapsedTime = 0.0f;
    float initialRadius = 0.0f; float finalRadius = node.radius;
    float initialFontSize = 0.0f; float finalFontSize = node.fontSize;
    
    while (elapsedTime < duration) {
        float t = elapsedTime / duration;
        float currentRadius = initialRadius + t * (finalRadius - initialRadius);
        float currentFontSize = initialFontSize + t * (finalFontSize - initialFontSize);
        node.radius = currentRadius;
        node.fontSize = currentFontSize;

        BeginDrawing();
        tool.draw();
        tool.UpdatePressOn();
        node.draw();
        draw();
        EndDrawing();

        elapsedTime += GetFrameTime();
    }
    
    node.radius = finalRadius;
    node.fontSize = finalFontSize;
}

void SinglyLinkedList::disappear(Node &node) {
    float duration = deltaTime;
    float elapsedTime = 0.0f;
    float initialRadius = node.radius;
    float initialFontSize = node.fontSize;

    while (elapsedTime < duration) {
        float t = elapsedTime / duration;
        float currentRadius = initialRadius - t * initialRadius;
        float currentFontSize = initialFontSize - t * initialFontSize;
        node.radius = currentRadius;
        node.fontSize = currentFontSize;

        BeginDrawing();
        tool.draw();
        tool.UpdatePressOn();
        updatePseudocodeOn();
        node.draw();
        draw();
        EndDrawing();

        elapsedTime += GetFrameTime();
    }
}

void SinglyLinkedList::animateConnection(Vector2 p1, Vector2 p2, Color color, float thickness) {
    float elapsedTime = 0.0f;
    float duration = deltaTime; 
    Vector2 start = GetCircleEdgePoint(p1, p2, radius);
    Vector2 end = GetCircleEdgePoint(p2, p1, radius);

    while (elapsedTime < duration) {
        float t = elapsedTime / duration;

        Vector2 currentEnd = {
            start.x + t * (end.x - start.x),
            start.y + t * (end.y - start.y)
        };

        BeginDrawing();

        tool.draw();
        tool.UpdatePressOn();

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

        EndDrawing();

        elapsedTime += GetFrameTime();
    }

    BeginDrawing();
    DrawLineEx(start, end, thickness, color);
    EndDrawing();
}

void SinglyLinkedList::animateDisconnection(Vector2 p1, Vector2 p2, Color color, float thickness) {
    float elapsedTime = 0.0f;
    float duration = deltaTime; 
    Vector2 start = GetCircleEdgePoint(p1, p2, radius);
    Vector2 end = GetCircleEdgePoint(p2, p1, radius);

    while (elapsedTime < duration) {
        float t = elapsedTime / duration;

        Vector2 currentEnd = {
            end.x - t * (end.x - start.x),
            end.y - t * (end.y - start.y)
        };

        BeginDrawing();

        tool.draw();
        tool.UpdatePressOn();

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

        EndDrawing();

        elapsedTime += GetFrameTime();
    }
}

void SinglyLinkedList::goingSt(int nidx) {
    bool isAdding = (currentPseudocode == addNodeAtHeadCode || 
                currentPseudocode == addNodeAtTailCode || 
                currentPseudocode == addNodeAtPositionCode);

    if (nidx == idx - 1) {
        int curOp = history[idx - 1].curop;
        if (curOp == 1) {
            animateListMovement(list, history[idx - 1].nodes);
        }
        else if (curOp == 2) {
            if (currentPseudocode == addNodeAtPositionCode) {
                list[history[idx - 1].pos].color = ORANGE;
                disappear(list[history[idx - 1].pos]);
                list[history[idx - 1].pos].color = BLACK;
            }
            else {
                Node tmp = pnode;
                pnode.color = ORANGE;
                disappear(pnode);
                pnode = tmp;
                visb = !visb;
            }
        }
        else if (curOp == 3) {
            list = history[idx - 1].nodes;
            list[history[idx - 1].pos].color = ORANGE;
            appear(list[history[idx - 1].pos]);
            list[history[idx - 1].pos].color = BLACK;
        }
        else if (curOp == 4) {
            list = history[idx - 1].nodes;
            if (history[idx].pos == 0) 
                animateDisconnection(history[idx].nodes[0].position, history[idx - 1].nodes[0].position, ORANGE, 4.0f);
            else if (history[idx].pos == history[idx].nodes.size()) 
                animateDisconnection(history[idx - 1].nodes.back().position, history[idx].nodes.back().position, ORANGE, 4.0f);
        }
    }
    else if (nidx == idx + 1) {
        int curOp = history[idx].curop;
        if (curOp == 1) {
            animateListMovement(list, history[idx + 1].nodes);
        }
        else if (curOp == 2) {
            if (currentPseudocode == addNodeAtPositionCode) {
                list = history[idx + 1].nodes;
                list[history[idx].pos].color = ORANGE;
                appear(list[history[idx].pos]);
                list[history[idx].pos].color = BLACK;
            }
            else {
                pnode.color = ORANGE;
                appear(pnode);
                pnode.color = BLACK;
                visb = 1;
            } 
        }
        else if (curOp == 3) {
            list[history[idx].pos].color = ORANGE;
            disappear(list[history[idx].pos]);
            list[history[idx].pos].color = BLACK;
        }
        else if (curOp == 4) {
            if (history[idx].pos == 0) 
                animateConnection(history[idx + 1].nodes[0].position, history[idx].nodes[0].position, ORANGE, 4.0f);
            else if (history[idx].pos == history[idx].nodes.size()) 
                animateConnection(history[idx].nodes.back().position, history[idx + 1].nodes.back().position, ORANGE, 4.0f);
        }
    }

    idx = nidx;
    list = history[idx].nodes;
    highlightedLine = history[idx].curline;

    if (idx == 0) visb = 0;
    else if (idx == history.size() - 1 && isAdding && currentPseudocode != addNodeAtPositionCode) {
        visb = 1;
    }

    cout << '\n' << idx << ' ' << visb << '\n';
    for (int i = 0; i < list.size(); i++) {
        cout << list[i].position.x << ' ';
    }

}

void SinglyLinkedList::running() {
    if (history.empty()) return;

    isRunning = true;
    remote[2] = {{remote[1].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/pause.png"),RAYWHITE,1};

    while (idx + 1 < history.size()) {
        if (history[idx].curop == 0) {
            idx ++;
            list = history[idx].nodes;
            highlightedLine = history[idx].curline;

            float duration = deltaTime;
            float elapsedTime = 0.0f;
            while (elapsedTime < duration) {
                elapsedTime += GetFrameTime();
                BeginDrawing();
                tool.draw();
                tool.UpdatePressOn();
                draw();
                EndDrawing();
            }
        }
        else goingSt(idx + 1);
    }

    isRunning = false;
    remote[2] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/play.png"),RAYWHITE,1};
}

void SinglyLinkedList::addNodeAtHead(std::string data) {
    if (!list.empty()) list = history.back().nodes;
    idx = 0, history.clear();

    currentPseudocode = addNodeAtHeadCode;

    if (!list.empty()) history.push_back({0, 1, 0, list});

    for (int i = 0; i < list.size(); i++) {
        Vector2 newPosition = generatePosition(i + 1);
        list[i].position = newPosition;
    }


    history.push_back({0, 2, 0, list});

    if (!list.empty()) history.push_back({1, 4, 0, list});
    
    Vector2 position = generatePosition(0);
    Node newNode = Node(data, position, BLACK, radius, customFont, fontSize);
    list.push_back(newNode);
    for (int i = list.size() - 1; i > 0; i--) list[i] = list[i - 1];
    list[0] = newNode;

    pnode = newNode;
    history.push_back({2, 0, 0, list});

    goingSt(0);
    running();
}

void SinglyLinkedList::addNodeAtTail(std::string data) {
    if (!list.empty()) list = history.back().nodes;
    idx = 0, history.clear();

    currentPseudocode = addNodeAtTailCode;

    history.push_back({0, 2, (int)list.size(), list});

    if (!list.empty()) history.push_back({1, 4, (int)list.size(), list});

    Vector2 position = generatePosition(list.size());
    Node newNode = Node(data, position, BLACK, radius, customFont, fontSize);
    list.push_back(newNode);

    pnode = newNode;
    history.push_back({2, 0, 0, list});

    goingSt(0);
    running();
}

void SinglyLinkedList::addNodeAtPosition(int pos, std::string data) {
    if (pos < 0 || pos > list.size()) return;

    if (pos == 0) {
        addNodeAtHead(data);
        return;
    } else if (pos == list.size()) {
        addNodeAtTail(data);
        return;
    }

    if (!list.empty()) list = history.back().nodes;
    idx = 0, history.clear();

    currentPseudocode = addNodeAtPositionCode;

    history.push_back({0, 0, 0, list});

    for (int i = 0; i <= pos; ++i) {
        list[i].color = ORANGE;
        history.push_back({0, 0, 0, list});
        list[i].color = BLACK;
    }

    history.push_back({1, 1, pos, list});

    for (int i = pos; i < list.size(); i ++)
        list[i].position = generatePosition(i + 1);

    history.push_back({1, 2, pos, list});

    Vector2 position = generatePosition(pos);
    Node newNode = Node(data, position, BLACK, radius, customFont, fontSize);
    list.push_back(newNode);
    for (int i = list.size() - 1; i > pos; --i) list[i] = list[i - 1];
    list[pos] = newNode;

    pnode = newNode;
    history.push_back({2, 0, 0, list});

    goingSt(0);
    running();
}

void SinglyLinkedList::deleteNodeAtHead() {
    if (!list.empty()) list = history.back().nodes;
    idx = 0, history.clear();

    currentPseudocode = deleteNodeAtHeadCode;

    history.push_back({0, 3, 0, list});

    for (int i = 0; i < list.size() - 1; i ++) list[i] = list[i + 1];
    list.pop_back();

    history.push_back({1, 1, 0, list});

    for (int i = 0; i < list.size(); i ++)
        list[i].position = generatePosition(i);

    history.push_back({1, 0, 0, list});

    goingSt(0);
    running();
}

void SinglyLinkedList::deleteNodeAtTail() {
    if (!list.empty()) list = history.back().nodes;
    idx = 0, history.clear();

    currentPseudocode = deleteNodeAtTailCode;

    history.push_back({0, 0, (int)list.size() - 1, list});
    history.push_back({1, 0, (int)list.size() - 1, list});

    for (int i = 0; i < list.size(); i ++) {
        list[i].color = ORANGE;
        history.push_back({2, 0, 0, list});
        list[i].color = BLACK;
    }

    history.push_back({3, 3, (int)list.size() - 1, list});

    list.pop_back();

    history.push_back({3, 0, 0, list});

    goingSt(0);
    running();
}

void SinglyLinkedList::deleteNode(int position) {
    if (position < 0 || position >= list.size()) return;
    
    if (position == 0) {
        deleteNodeAtHead();
        return;
    } else if (position == list.size() - 1) {
        deleteNodeAtTail();
        return;
    }

    if (!list.empty()) list = history.back().nodes;
    idx = 0, history.clear();

    currentPseudocode = deleteNodeAtPositionCode;

    history.push_back({0, 0, 0, list});

    for (int i = 0; i < position; ++i) {
        list[i].color = ORANGE;
        history.push_back({1, 0, 0, list});
        list[i].color = BLACK;
    }

    list[position].color = ORANGE;
    history.push_back({2, 0, position, list});
    history.push_back({3, 3, position, list});

    for (int i = position; i < list.size() - 1; ++i) 
        list[i] = list[i + 1];
    list.pop_back();
    
    history.push_back({3, 1, position, list});

    for (int i = 0; i < list.size(); ++i) 
        list[i].position = generatePosition(i);
    
    history.push_back({3, 0, 0, list});

    goingSt(0);
    running();
}

void SinglyLinkedList::createRandomList(int n) {
    clear();
    visb = 0;
    for (int i = 0; i < n; ++i) {
        int data = rand() % 100;
        addNodeAtTail(std::to_string(data));
    }
}

void SinglyLinkedList::loadFile() {
    const char* filepath = tinyfd_openFileDialog(
        "Choose file",    
        "",                
        0, NULL,           
        NULL,              
        0                  
    );

    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error.\n");
        return;
    }

    clear();

    int n;
    fscanf(file, "%d", &n);
    
    for (int i = 0; i < n; i++) {
        int u;
        fscanf(file, "%d", &u);
        std::string data = std::to_string(u);
        addNodeAtTail(data);
    }

    fclose(file);
}

void SinglyLinkedList::clear() {
    currentPseudocode = nullptr;
    highlightedLine = -1;

    list.clear();
    history.clear();
}

void SinglyLinkedList::checkPause() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (remote[2].CheckMouse(mousePos, 10)) {
            isRunning = !isRunning;
        }
    }

    if (isRunning) {
        remote[2] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/pause.png"),RAYWHITE,1};
    }
    else {
        remote[2] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/play.png"),RAYWHITE,1};
    }
}

void SinglyLinkedList::handleRemote() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        for (int i = 0; i < remote.size(); i++) {
            button &v = remote[i];
            if (v.CheckMouse(mousePos, 10)) {
                if (i == 0) {
                    goingSt(0);
                } else if (i == 1) {
                    if (idx - 1 >= 0) goingSt(idx - 1);
                } else if (i == 2) {
                    checkPause();
                    if (isRunning) running();
                } else if (i == 3) {
                    if (idx + 1 < history.size()) goingSt(idx + 1);
                } else if (i == 4) {
                    goingSt(history.size() - 1);
                }
            }
        }
    }
}

void SinglyLinkedList::handleEvents() {
    Vector2 mousePosition = GetMousePosition();
    updatePseudocodeOn();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePosition, togglePseudocodeButton)) {
            isPseudocodeVisible = !isPseudocodeVisible;
        }

        if (CheckCollisionPointRec(mousePosition, loadFileButton)) {
            loadFile();
        }

        if (CheckCollisionPointRec(mousePosition, addPosButton)) {
            inputActive = true;
            inputType = 5;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        if (CheckCollisionPointRec(mousePosition, addHeadButton)) {
            inputActive = true;
            inputType = 1;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        if (CheckCollisionPointRec(mousePosition, addTailButton)) {
            inputActive = true;
            inputType = 2;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        if (CheckCollisionPointRec(mousePosition, deleteButton)) {
            inputActive = true;
            inputType = 3;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        if (CheckCollisionPointRec(mousePosition, createRandomButton)) {
            inputActive = true;
            inputType = 4;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        if (CheckCollisionPointRec(mousePosition, clearButton)) {
            clear();
        }
    }

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

    handleRemote();
}