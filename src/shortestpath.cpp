#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/tinyfiledialogs.h"
#include "../include/var.h"
#include "../include/ui.h"
#include "../include/node.h"
#include "../include/manager.h"
#include "../include/shortestpath.h"

void ShortestPath::updateEdgeLengths() {
    for (Edge &edge : edges) {
        edge.restLength = edgeLength;
    }
}

void ShortestPath::updatePhysics(float stiffness, float damping) {
    if (IsKeyPressed(KEY_UP)) {
        edgeLength += 10.0f;
        desiredDistance += 10.0f; 
        updateEdgeLengths();
    }
    if (IsKeyPressed(KEY_DOWN)) {
        edgeLength -= 10.0f;
        desiredDistance -= 10.0f;
        updateEdgeLengths();
    }

    for (Edge &edge : edges) {
        Node &nodeA = nodes[edge.nodeA];
        Node &nodeB = nodes[edge.nodeB];

        Vector2 delta = { nodeB.position.x - nodeA.position.x, nodeB.position.y - nodeA.position.y };
        float distance = sqrt(delta.x * delta.x + delta.y * delta.y);

        if (distance == 0.0f) continue;

        float diff = (distance - edge.restLength) / distance;
        Vector2 force = { delta.x * stiffness * diff, delta.y * stiffness * diff };

        if (!nodeA.isDragging) {
            nodeA.velocity.x += force.x;
            nodeA.velocity.y += force.y;
        }
        if (!nodeB.isDragging) {
            nodeB.velocity.x -= force.x;
            nodeB.velocity.y -= force.y;
        }
    }

    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            Vector2 delta = { nodes[j].position.x - nodes[i].position.x, nodes[j].position.y - nodes[i].position.y };
            float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
            if (distance == 0.0f) continue;

            float diff = (distance - desiredDistance) / distance;
            Vector2 force = { delta.x * stiffness * diff, delta.y * stiffness * diff };

            if (!nodes[i].isDragging) {
                nodes[i].velocity.x += force.x;
                nodes[i].velocity.y += force.y;
            }
            if (!nodes[j].isDragging) {
                nodes[j].velocity.x -= force.x;
                nodes[j].velocity.y -= force.y;
            }
        }
    }

    for (Node &node : nodes) {
        node.velocity.x *= damping;
        node.velocity.y *= damping;

        if (!node.isDragging) {
            node.position.x += node.velocity.x;
            node.position.y += node.velocity.y;
        }

        node.position.x = max(node.position.x, (float)195);
        node.position.x = min(node.position.x, (float)screenWidth - 65);
        node.position.y = max(node.position.y, (float)83);
        node.position.y = min(node.position.y, (float)710);
    }
}

void ShortestPath::init() {
    createButton = { 20, 460, 150, 50 };
    loadFileButton = { 20, 520, 150, 50 };
    dijkstraButton = { 20, 580, 150, 50 };
    clearButton = { 20, 640, 150, 50 };

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

void ShortestPath::updatePseudocodeOn() {
    Vector2 mousePosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePosition, togglePseudocodeButton)) {
            isPseudocodeVisible = !isPseudocodeVisible; 
        }
    }
}

void ShortestPath::drawPseudocode(const char** pseudocode, int lineCount, int highlightedLine) {
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

void ShortestPath::addNode(Vector2 position) {
    Node newNode = Node(std::to_string(nodes.size()), position, BLACK, radius, customFont, fontSize, {0, 0});
    nodes.push_back(newNode);  
}

void ShortestPath::clear() {
    nodes.clear();
    edges.clear();
    dist.clear();
    curdist.clear();
    currentPseudocode = nullptr;
}

void ShortestPath::animateConnection(Vector2 p1, Vector2 p2, Color color, float thickness) {
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
        draw();
        DrawLineEx(start, currentEnd, thickness, color);

        EndDrawing();

        elapsedTime += GetFrameTime(); 
    }

    BeginDrawing();
    DrawLineEx(start, end, thickness, color);
    EndDrawing();
}

void ShortestPath::draw() {
    DrawRectangleRec(createButton, LIGHTGRAY);
    DrawTextEx(customFont, "Create (V, E)", { createButton.x + 10, createButton.y + 15 }, 20, 2, BLACK);
    
    DrawRectangleRec(loadFileButton, LIGHTGRAY);
    DrawTextEx(customFont, "Load File", { loadFileButton.x + 10, loadFileButton.y + 15 }, 20, 2, BLACK);

    DrawRectangleRec(dijkstraButton, LIGHTGRAY);
    DrawTextEx(customFont, "Dijkstra", { dijkstraButton.x + 10, dijkstraButton.y + 15 }, 20, 2, BLACK);

    DrawRectangleRec(clearButton, LIGHTGRAY);
    DrawTextEx(customFont, "Clear", { clearButton.x + 10, clearButton.y + 15 }, 20, 2, BLACK);

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
        if (inputType == 1) {
            DrawRectangle(180, 460, 100, 50, LIGHTGRAY);
            DrawTextEx(customFont, inputBuffer, { 190, 475 }, 20, 2, BLACK);
        }
        else {
            DrawRectangle(180, 580, 100, 50, LIGHTGRAY);
            DrawTextEx(customFont, inputBuffer, { 190, 595 }, 20, 2, BLACK);
        }
    }

    updatePhysics();

    for (const Edge &edge : edges) {
        DrawConnection(nodes[edge.nodeA].position, nodes[edge.nodeB].position, 0, edge.color, 4.0f);
        DrawTextEx(customFont, std::to_string(edge.Weight).c_str(), 
                    {(nodes[edge.nodeA].position.x + nodes[edge.nodeB].position.x) / 2, 
                     (nodes[edge.nodeA].position.y + nodes[edge.nodeB].position.y) / 2}, 
                    18, 2, edge.color);
    }

    for (Node &node : nodes) {
        node.draw();
    }

    for (int i = 0; i < curdist.size(); i++) {
        if (curdist[i] == INT_MAX) {
            DrawTextEx(customFont, "inf", 
                    {nodes[i].position.x - 8, nodes[i].position.y + 30}, 20, 2, RED);
        }
        else if (curdist[i] == -1) {
            DrawTextEx(customFont, "source, 0", 
                {nodes[i].position.x - 30, nodes[i].position.y + 30}, 20, 2, RED);
        }
        else {
            DrawTextEx(customFont, std::to_string(curdist[i]).c_str(), 
                    {nodes[i].position.x - 7.5f, nodes[i].position.y + 30}, 20, 2, RED);
        }   
    }
}

void ShortestPath::create(int n, int m) {
    clear();

    for (int i = 0; i < n; i++) {
        Vector2 position = {(float)Rand(200, screenWidth - 200), (float)Rand(200, screenHeight - 200)};

        addNode(position);
    }

    vector <vector<int>> isEdgeCreated(n, vector<int>(n, 0));
    m = min(m, n * (n - 1) / 2); 

    for (int i = 0; i < m; i++) {
        int nodeA = Rand(0, n - 1);
        int nodeB = Rand(0, n - 1);
        int weight = Rand(1, 100);
        if (nodeA != nodeB && isEdgeCreated[nodeA][nodeB] == 0) {
            edges.push_back({nodeA, nodeB, weight, edgeLength, BLACK});
            isEdgeCreated[nodeA][nodeB] = 1;
            isEdgeCreated[nodeB][nodeA] = 1;
        }
        else i--;
    }
}

void ShortestPath::dijkstra(int s) {
    dist.clear();
    curdist.clear();
    dist.resize(nodes.size(), INT_MAX);
    curdist.resize(nodes.size(), INT_MAX);
    vector<int> parent(nodes.size(), -1);
    vector<bool> visited(nodes.size(), false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[s] = 0;
    curdist[s] = -1;
    pq.push({0, s});

    vector <int> h;
    while (!pq.empty()) {
        auto [currentDist, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        h.push_back(u);

        if (currentDist > dist[u]) continue;

        for (const Edge &edge : edges) {
            if (edge.nodeA == u || edge.nodeB == u) {
                int v = u ^ edge.nodeA ^ edge.nodeB;
                float weight = edge.Weight;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    spEdges.clear();
    for (int u : h) {
        if (parent[u] != -1) {
            spEdges.push_back({parent[u], u, dist[u], edgeLength, ORANGE, true});
        }
    }

    for (Edge &edge : spEdges) {
        for (int i = 0; i < edges.size(); i++) {
            if ((edges[i].nodeA == edge.nodeA && edges[i].nodeB == edge.nodeB) ||
                (edges[i].nodeA == edge.nodeB && edges[i].nodeB == edge.nodeA)) {
                edge.Weight = i;
                break;
            }
        }
    }
}

void ShortestPath::checkPause() {
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

void ShortestPath::goingSt(int nidx) {
    if (nidx == idx - 1) {
        animateConnection(nodes[spEdges[idx - 1].nodeB].position, nodes[spEdges[idx - 1].nodeA].position, BLACK, 4.0f);
        edges[idx - 1].color = BLACK;
    }
    else if (nidx == idx + 1) {
        Color tmp = spEdges[idx].isSelected ? ORANGE : LIGHTGRAY;
        animateConnection(nodes[spEdges[idx].nodeA].position, nodes[spEdges[idx].nodeB].position, tmp, 4.0f);
    }

    idx = nidx;

    for (int i = 0; i < nodes.size(); i++) {
        nodes[i].color = BLACK;
        if (curdist[i] != -1) curdist[i] = INT_MAX;
    }
    for (Edge &edge : edges) {
        edge.color = BLACK;
    }

    vector <bool> visited(nodes.size(), false);
    for (int i = 0; i < spEdges.size(); i++) {
        int id = spEdges[i].Weight;
        if (i < idx) {
            visited[spEdges[i].nodeA] = true;
            edges[id].color = ORANGE;
            nodes[edges[id].nodeA].color = ORANGE;
            nodes[edges[id].nodeB].color = ORANGE;
            curdist[edges[id].nodeA] = min(curdist[edges[id].nodeA], dist[edges[id].nodeA]);
            curdist[edges[id].nodeB] = min(curdist[edges[id].nodeB], dist[edges[id].nodeB]);
        } else {
            edges[id].color = BLACK;
        }
    }

    highlightedLine = 1;
    if (visited[spEdges[idx].nodeA]) {
        highlightedLine = 2;
    }
    if (idx == spEdges.size()) {
        highlightedLine = 3;
    }
}

void ShortestPath::running() {
    if (nodes.empty()) return;

    vector <bool> visited(nodes.size(), false);
    for (int &i = idx; i < spEdges.size() && isRunning; i++) {
        Edge spEdge = spEdges[i];
        int id = spEdge.Weight;
        Edge edge = edges[id];

        if (!visited[spEdge.nodeA]) {
            highlightedLine = 1;
            animateConnection(nodes[edges[0].nodeA].position, nodes[edges[0].nodeB].position, edges[0].color, 4.0f);
        }
       
        highlightedLine = 2;
        animateConnection(nodes[edge.nodeA].position, nodes[edge.nodeB].position, edge.color, 4.0f);
    
        highlightedLine = 3;
        visited[spEdge.nodeA] = true;
        nodes[edge.nodeA].color = ORANGE;
        nodes[edge.nodeB].color = ORANGE;
        curdist[edge.nodeA] = min(curdist[edge.nodeA], dist[edge.nodeA]);
        curdist[edge.nodeB] = min(curdist[edge.nodeB], dist[edge.nodeB]);
        edge.color = ORANGE;

        animateConnection(nodes[spEdge.nodeA].position, nodes[spEdge.nodeB].position, edge.color, 4.0f);
        edges[id].color = edge.color;
    }

    isRunning = false;
    remote[2] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/play.png"),RAYWHITE,1};
}

void ShortestPath::findSP(int s) {
    if (nodes.empty() || s + 1 > nodes.size()) return;

    dijkstra(s);
    
    currentPseudocode = dijkstraCode;
    highlightedLine = 0; // Highlight the first line of pseudocode
    idx = 0;
    
    isRunning = true;
    remote[2] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/pause.png"),RAYWHITE,1};
    
    goingSt(0);
    running();
}

void ShortestPath::loadFile() {
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

    int n, m;
    fscanf(file, "%d %d", &n, &m);
    create(n, m);
    edges.clear();
    for (int i = 0; i < m; i++) {
        int u, v, w;
        fscanf(file, "%d %d %d", &u, &v, &w);
        edges.push_back({u, v, w, edgeLength, BLACK});
    }

    fclose(file);
}

void ShortestPath::handleRemote() {
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
                    if (idx + 1 <= spEdges.size()) goingSt(idx + 1);
                } else if (i == 4) {
                    goingSt(spEdges.size());
                }
            }
        }
    }
}

void ShortestPath::handleMouseInput() {
    Vector2 mousePos = GetMousePosition();
    for (Node &node : nodes) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointCircle(mousePos, node.position, node.radius)) {
            node.isDragging = true;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            node.isDragging = false;
        }
        if (node.isDragging) {
            node.position = mousePos;
            node.velocity = { 0, 0 };
        }
    }
}

void ShortestPath::handleEvents() {
    handleMouseInput();

    Vector2 mousePosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePosition, loadFileButton)) {
            loadFile();
        }
        if (CheckCollisionPointRec(mousePosition, createButton)) {
            inputActive = true;
            inputType = 1;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }
        if (CheckCollisionPointRec(mousePosition, dijkstraButton)) {
            inputActive = true;
            inputType = 2;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }
        if (CheckCollisionPointRec(mousePosition, clearButton)) {
            clear();
        }
    }

    if (inputActive) {
        int key = GetCharPressed();
        while (key > 0) {
            if (((key >= 48) && (key <= 57) && strlen(inputBuffer) < 9) || (key == 32)) {
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
            string value = inputBuffer;
            if (inputType == 1) {
                int n = 0, m = 0, j = 0;
                while (j < value.size() && value[j] != ' ') n = n * 10 + value[j++] - '0';
                j++; 
                
                if (j >= value.size()) m = Rand(n, n*(n+1)/2);
                else for (int i = j; i < value.size(); i++)
                        if (value[j] != ' ') m = m * 10 + value[i] - '0';

                create(n, m);
                
                inputActive = false;
            }
            else if (inputType == 2) {
                int n = 0, j = 0;
                while (j < value.size() && value[j] != ' ') n = n * 10 + value[j++] - '0';

                findSP(n);

                inputActive = false;
            }
        }
    }

    handleRemote();
}
