#include <raylib.h>
#include "../include/trie.h"
#include <iostream>


Trie::Trie(){
    
}

void Trie::init(){
    int buttonWidth = 120, buttonHeight = 30;
    int offsetX = 15, offsetY = 120 + 600 - 7*buttonHeight - 6 * 5;
    createButton  = ButtonText(offsetX, offsetY, buttonWidth, buttonHeight, "Create");
    searchButton = ButtonText(offsetX, offsetY + buttonHeight + 5, buttonWidth, buttonHeight, "Search");
    addButton = ButtonText(offsetX, offsetY + 2*buttonHeight + 10, buttonWidth, buttonHeight, "Insert");    
    deleteButton = ButtonText(offsetX, offsetY + 3*buttonHeight + 15, buttonWidth, buttonHeight, "Delete");
    updateButton = ButtonText(offsetX, offsetY + 4*buttonHeight + 20, buttonWidth, buttonHeight, "Update");
    fileButton = ButtonText(offsetX, offsetY + 5*buttonHeight + 25, buttonWidth, buttonHeight, "File");
    clearButton = ButtonText(offsetX, offsetY + 6*buttonHeight + 30, buttonWidth, buttonHeight, "Clear");

    createTextBox = InputTextBox({createButton.bounds.x + createButton.bounds.width + 10, createButton.bounds.y, 150, createButton.bounds.height}, "N = ", 2, 20, BLACK, BLACK, LIGHTGRAY);
    searchTextBox = InputTextBox({searchButton.bounds.x + searchButton.bounds.width + 10, searchButton.bounds.y, 150, searchButton.bounds.height}, "S = ", 7, 20, BLACK, BLACK, LIGHTGRAY);
    addTextBox = InputTextBox({addButton.bounds.x + addButton.bounds.width + 10, addButton.bounds.y, 150, addButton.bounds.height}, "S = ", 7, 20, BLACK, BLACK, LIGHTGRAY);
    deleteTextBox = InputTextBox({deleteButton.bounds.x + deleteButton.bounds.width + 10, deleteButton.bounds.y, 150, deleteButton.bounds.height}, "S = ", 7, 20, BLACK, BLACK, LIGHTGRAY);
    updateUTextBox = InputTextBox({updateButton.bounds.x + updateButton.bounds.width + 10, updateButton.bounds.y, 150, updateButton.bounds.height}, "U = ", 7, 20, BLACK, BLACK, LIGHTGRAY);
    updateVTextBox = InputTextBox({updateButton.bounds.x + updateButton.bounds.width + 10 + 125, updateButton.bounds.y, 150, updateButton.bounds.height}, "V = ", 7, 20, BLACK, BLACK, LIGHTGRAY);


    remote.resize(7);
    remote[0] = {{350,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/pre.png"),RAYWHITE,1};    
    remote[1] = {{remote[0].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/undo.png"),RAYWHITE,1};
    remote[2] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/play.png"),RAYWHITE,1};
    remote[3] = {{remote[2].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/redo.png"),RAYWHITE,1};
    remote[4] = {{remote[3].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/suff.png"),RAYWHITE,1};
    remote[5] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/pause.png"),RAYWHITE,1};
    remote[6] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/repeat.png"),RAYWHITE,1};

    resetTrie(MAX_NODES);

    animationState = PAUSE;
    timeAnimation = 0.0f;
    curAnimation = 0;
    Animation.clear();

    inputActive = false;    // Flag to check if input is active
    inputType = 0;         // Type of input (0: None, 1: Create, 2: Search, 3: Insert, 4: Delete)

    lineCount = 8;
    isPseudocodeVisible = false;
    togglePseudocodeButton = { (float)screenWidth-20, 490, 20, (float)lineCount * 30};
}

void Trie::updatePseudocodeOn() {
    Vector2 mousePosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePosition, togglePseudocodeButton)) {
            isPseudocodeVisible = !isPseudocodeVisible; 
        }
    }
}

void Trie::drawPseudocode(const char** pseudocode, int codeLine1, int codeLine2, int codeLine3) {
    float startX = (float)screenWidth - 450 - 5 - 30;
    float startY = 490;

    float boxWidth = 470;
    float boxHeight = 30;

    Rectangle pseudocodeBox = { 
        startX, 
        startY, 
        boxWidth, 
        boxHeight * (float)lineCount 
    };
    DrawRectangleRec(pseudocodeBox, LIGHTGRAY);
    if(pseudocode == nullptr) return;

    for(int i = 0; i < lineCount; ++i){
        float newX = startX;
        float newY = startY + i * boxHeight;
        Rectangle textBox = {newX, newY + 5, boxWidth, boxHeight - 9};
        Color backgroundColor = (i == codeLine1 || i == codeLine2 || i == codeLine3) ? GREEN : LIGHTGRAY;
        DrawRectangleRec(textBox, backgroundColor);

        Color textColor = (i == codeLine1 || i == codeLine2 || i == codeLine3) ? WHITE : BLACK;
        DrawTextEx(fontPseudocode, pseudocode[i], { newX + 5, newY + 5 }, 20, 2, textColor);
    }
}

void Trie::handleRemote(){
    button &curButton = remote[2];
    if(animationState == PAUSE){
        if(curButton.CheckPress(GetMousePosition(), 10, IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
            animationState = PLAY;
        }
    }
    else if(animationState == PLAY){
        if(curButton.CheckPress(GetMousePosition(), 10, IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
            animationState = PAUSE;
        }
    }
    else if(animationState == REPLAY){
        if(curButton.CheckPress(GetMousePosition(), 10, IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
            animationState = PLAY;
            curAnimation = 0;
            timeAnimation = 0.0f;
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (int i = 0; i < remote.size(); ++i) {
            if (remote[i].CheckMouse(GetMousePosition(), 10)) {
                if (i == 0) { // Go to beginning
                    curAnimation = 0;
                    timeAnimation = 0.0f;
                    animationState = PAUSE;
                } 
                else if (i == 1) { // Undo button
                    if (curAnimation > 0) {
                        curAnimation--;
                        timeAnimation = 0.0f;
                        animationState = PAUSE;
                    }
                } 
                else if (i == 3) { // Redo button
                    if (curAnimation + 1 < Animation.size()) {
                        curAnimation++;
                        timeAnimation = 0.0f;
                        animationState = PAUSE;
                    }
                } 
                else if (i == 4) { // Go to end
                    curAnimation = Animation.size() - 1;
                    timeAnimation = 0.0f;
                    animationState = REPLAY;
                }
            }
        }
    }

    Vector2 Mouse = GetMousePosition();
    for (int i = 0; i < 5; ++i) {
        if(i == 2) continue;
        if (remote[i].CheckMouse(Mouse,10)) remote[i].col = 1;
        else remote[i].col = 0;
        remote[i].DrawBasic(0.6);
    }

    if (animationState == PLAY) remote[2].DrawBasic(0.6);
    else if (animationState == PAUSE) remote[5].DrawBasic(0.6);
    else if (animationState == REPLAY) remote[6].DrawBasic(0.6);
}

float Trie::generatePosition(int u, float t, float f, float h, float delh, float r, int isSavePos) {
    if(u == -1) return false;

    float g = t;
    float mid = -1;
    float totalChildPos = 0;
    int childCount = 0;
    for(int i = 0; i < 26; ++i){
        int v = nodes[u].child[i];
        if(v == -1) continue;
        g = generatePosition(v, g, f, h + delh, delh, r, isSavePos);
        totalChildPos += nodes[v].node.position.x; // Cộng dồn vị trí x của các nút con
        childCount++;
    }

    if(childCount > 0){
        mid = totalChildPos / childCount;
    } else {
        // mid = (std::max(g - f - 2 * r, t) + t) / 2.0f; // Nếu không có nút con, tính như cũ
        // mid = g + f + r;
        mid = t;
    }

    if(u == 0) h -= 70;
    if(isSavePos == 1){
        nodes[u].oldPosition = nodes[u].newPosition;
        nodes[u].newPosition = {mid, h};
    }
    
    nodes[u].node.position = {mid, h};
    nodes[u].node.radius = r;

    return std::max(g, mid + 3 * r);
}

void Trie::adjustPosition(){
    float r = radius;
    float dis = 30;
    // float k = screenWidth;
    float k = 0;
    for(int i = r; i >= 5; --i){
        dis -= 3;
        r -= 0.5;
        k = screenWidth - (generatePosition(0, 0, dis, 170, 70, r, 0) - 3*r);
        if(k > 30) break;
    }
    generatePosition(0, k/2.0f, dis, 170, 70, r, 1);
    nodes[0].oldPosition = nodes[0].newPosition;
    // generatePosition(0, k/2.0f, dis, 120, 70, r, 1);
}

Vector2 Trie::newPosVector2(Vector2 A, Vector2 B, float opacity){
    return {A.x + (B.x - A.x) * opacity, A.y + (B.y - A.y) * opacity};
}

void Trie::drawAnimation(SnapShot curShot, float opacity){
    if(isPseudocodeVisible){
        drawPseudocode(curShot.code, curShot.codeLine1, curShot.codeLine2, curShot.codeLine3); // Draw the pseudocode
    }
    if(!isLightMode){
        nodeColor = ((1) ? Color({200, 200, 200, 255}) : Color({220, 220, 220, 255}));
        nodeHighlightColor = ((1) ? Color({255, 191, 73, 255}) : Color({255, 184, 108, 255}));
        nodeDeleteColor = ((1) ? Color({255, 99, 99, 255}) : Color({255, 80, 80, 255}));
        nodeExistColor = ((1) ? Color({144, 238, 144, 255}) : Color({102, 255, 102, 255}));   
    }
    else{
        nodeColor = BLACK;
        nodeHighlightColor = ORANGE;
        nodeDeleteColor = RED;
        nodeExistColor = GREEN;
    }

    for(int i = 0; i < curShot.nodes.size(); ++i){
        TrieNode &u = curShot.nodes[i];
        if(i != 0){
            if(curShot.isInsertState && nodes[i].cnt == 0) continue;
            if(!curShot.isInsertState && curShot.nodes[i].cnt == 0) continue;
        }
        if(animationState == PAUSE) u.node.position = u.newPosition;
        else u.node.position = newPosVector2(u.oldPosition, u.newPosition, opacity);
        switch (u.typeColor) {
            case 0: u.node.color = nodeColor; break; // Default color
            case 1: u.node.color = nodeHighlightColor; break; // Highlight color
            case 2: u.node.color = nodeDeleteColor; break; // Delete color
            case 3: u.node.color = nodeExistColor; break; // Exist color
            default: u.node.color = BLACK; break; // Default color
        }
        u.node.draw(1.0f);
    }
    for(int i = 0; i < curShot.nodes.size(); ++i){
        TrieNode &u = curShot.nodes[i];
        for(int j = 0; j < 26; ++j){
            if(u.child[j] == -1) continue;
            TrieNode &v = curShot.nodes[u.child[j]];
            if(u.typeColor == v.typeColor) DrawConnection(u.node.position, v.node.position, true, u.node.color, 3.0f, u.node.radius, v.node.radius);
            else DrawConnection(u.node.position, v.node.position, true, nodeColor, 3.0f, u.node.radius, v.node.radius);
        }
    }
}

void Trie::drawTrie(int u){
    if(u == -1) return;
    nodes[u].node.draw();
    for(int i = 0; i < 26; ++i){
        int v = nodes[u].child[i];
        if(v == -1) continue;
        DrawConnection(nodes[u].node.position, nodes[v].node.position, true, BLACK, 4.0f, nodes[u].node.radius, nodes[v].node.radius);
        drawTrie(v);
    }
}

void Trie::draw(){
    handleRemote();
    // drawTrie(0);
    if(Animation.size() > 0){
        if (animationState != PAUSE) timeAnimation += GetFrameTime(); // Update the animation time
        if(timeAnimation > deltaTime && animationState != PAUSE){
            if(curAnimation + 1 < Animation.size()){
                curAnimation++; // Move to the next animation frame
                timeAnimation = 0; // Reset the animation time
            }
            else{
                timeAnimation = deltaTime; // Reset the animation time
                animationState = REPLAY;
            }
        }

        float opacity = std::min((double)1.0, timeAnimation / deltaTime); // Calculate the scale for the animation
        drawAnimation(Animation[curAnimation], opacity); // Draw the current animation frame
    }
    else{
        if(isPseudocodeVisible){
            drawPseudocode(nullptr, -1, -1, -1); // Draw the pseudocode
        }
    }

    drawButtons();
}

void Trie::createRandomTrie(int n){
    clear();
    for(int i = 0; i < n; ++i) {
        int len = GetRandomValue(1, 7);
        std::string s = "";
        for(int j = 0; j < len; ++j){
            s += (char)(GetRandomValue(0, 25) + 'A');
        }
        listStr.push_back(s);
        addString(s);
        adjustPosition();
    }
}

void Trie::insert(std::string word){
    if(word.empty()) return;
    Animation.clear();
    curAnimation = 0; // Reset the current animation index
    timeAnimation = 0.0f; // Reset the current time    
    animationState = PLAY;
    listStr.push_back(word);
    addString(word);    
    adjustPosition();
}

void Trie::search(std::string word){
    if(word.empty()) return;
    Animation.clear();
    curAnimation = 0; 
    timeAnimation = 0.0f;
    animationState = PLAY;
    searchString(word);
}

void Trie::remove(std::string word){
    if(word.empty()) return;
    listStr.erase(std::remove(listStr.begin(), listStr.end(), word), listStr.end());
    Animation.clear();
    curAnimation = 0; 
    timeAnimation = 0.0f;
    animationState = PLAY;
    deleteString(word);
    adjustPosition();
}

void Trie::update(std::string wordU, std::string wordV){
    Animation.clear();
    curAnimation = 0; 
    timeAnimation = 0.0f;
    animationState = PLAY;
    if(!wordU.empty()){
        listStr.erase(std::remove(listStr.begin(), listStr.end(), wordU), listStr.end());
        deleteString(wordU);
        adjustPosition();
        Animation.push_back(SnapShot(nodes, deleteCode, -1, -1, -1));
        adjustPosition();
    }
    if(!wordV.empty()){
        listStr.push_back(wordV);
        addString(wordV);
        adjustPosition();
    }
}

void Trie::clear(){
    Animation.clear();
    curAnimation = 0; // Reset the current animation index
    timeAnimation = 0.0f; // Reset the current time    
    animationState = PLAY;
    listStr.clear();
    resetTrie(MAX_NODES);
    adjustPosition();
}

void Trie::loadFile(){
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
    for(int i = 0; i < n; ++i){
        char str[100];
        fscanf(file, "%s", str); // Read the strings from the file
        std::string s = str;
        listStr.push_back(s);
        addString(s);
        adjustPosition();
    }
    fclose(file);
}

void Trie::handleEvents(){
    Vector2 mousePosition = GetMousePosition(); // Get the mouse position

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        // Check if the create button is clicked
        if (createButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 1;
            createTextBox.createRandomValue(1, 10);
        }
        else if (searchButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 2; // Set input type to search
            if(!listStr.empty()) searchTextBox.SetText(listStr[GetRandomValue(0, listStr.size() - 1)]);
            else searchTextBox.createRandomString(GetRandomValue(1, 7));
        }

        // Check if the insert button is clicked
        else if (addButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 3; // Set input type to insert
            addTextBox.createRandomString(GetRandomValue(1, 7));
        }

        // Check if the delete button is clicked
        else if (deleteButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 4; // Set input type to delete
            if(!listStr.empty()) deleteTextBox.SetText(listStr[GetRandomValue(0, listStr.size() - 1)]);
            else deleteTextBox.createRandomString(GetRandomValue(1, 7));
        }

        else if (updateButton.IsHovered(mousePosition)){
            inputActive = true;
            inputType = 5;
            if(!listStr.empty()) updateUTextBox.SetText(listStr[GetRandomValue(0, listStr.size() - 1)]);
            else updateUTextBox.createRandomString(GetRandomValue(1, 7));
            updateVTextBox.createRandomString(GetRandomValue(1, 7));
        }

        //Check if the file button is clicked
        else if (fileButton.IsHovered(mousePosition)) {
            loadFile();
        }

        else if (clearButton.IsHovered(mousePosition)) {
            clear(); // Clear the hash table
        }
    }

    if(inputActive){
        if(IsKeyPressed(KEY_ENTER)){
            if(inputType == 1){
                createTextBox.Update();                
                createRandomTrie(createTextBox.GetIntValue());
            }
            else if(inputType == 2){
                searchTextBox.Update();
                searchTextBox.reGenerateString();
                search(searchTextBox.GetText());
            }
            else if(inputType == 3){
                addTextBox.Update();
                addTextBox.reGenerateString();
                insert(addTextBox.GetText());
            }
            else if(inputType == 4){
                deleteTextBox.Update();
                deleteTextBox.reGenerateString();
                remove(deleteTextBox.GetText()); // Update to use GetText for deletion
            }
            else if(inputType == 5){
                updateUTextBox.Update();
                updateVTextBox.Update();
                updateUTextBox.reGenerateString();
                updateVTextBox.reGenerateString();
                update(updateUTextBox.GetText(), updateVTextBox.GetText()); 
            }
            inputActive = false; // Deactivate input after processing
            inputType = 0; // Reset input type
        }
    }
}

void Trie::drawButtons(){
    updatePseudocodeOn();
    addButton.Update();
    searchButton.Update();
    deleteButton.Update();
    updateButton.Update();
    createButton.Update();
    fileButton.Update();
    clearButton.Update();

    addButton.Draw();
    searchButton.Draw();
    deleteButton.Draw();
    updateButton.Draw();
    createButton.Draw();
    fileButton.Draw();
    clearButton.Draw();

    if(inputActive){
        if(inputType == 1){
            createTextBox.Update();
            createTextBox.Draw();
        }
        else if(inputType == 2){
            searchTextBox.Update();
            searchTextBox.reGenerateString();
            searchTextBox.Draw();
        }
        else if(inputType == 3){
            addTextBox.Update();
            addTextBox.reGenerateString();
            addTextBox.Draw();
        }
        else if(inputType == 4){
            deleteTextBox.Update();
            deleteTextBox.reGenerateString();
            deleteTextBox.Draw();
        }
        else if(inputType == 5){
            updateUTextBox.Update();                      
            updateVTextBox.Update();
            updateUTextBox.reGenerateString();
            updateVTextBox.reGenerateString();
            updateUTextBox.Draw();
            updateVTextBox.Draw();
        }
    }

    DrawRectangleRec(togglePseudocodeButton, GREEN);
    if (isPseudocodeVisible) {
        DrawTextEx(customFont, ">", { togglePseudocodeButton.x + (togglePseudocodeButton.width - MeasureText(">", 20)) / 2, togglePseudocodeButton.y + (togglePseudocodeButton.height - 20) / 2 }, 20, 2, WHITE);
    } else {
        DrawTextEx(customFont, "<", { togglePseudocodeButton.x + (togglePseudocodeButton.width - MeasureText("<", 20)) / 2, togglePseudocodeButton.y + (togglePseudocodeButton.height - 20) / 2 }, 20, 2, WHITE);
    }
}

Trie::~Trie(){
    // for(button v: remote){
    //     UnloadTexture(v.image);
    // }
}