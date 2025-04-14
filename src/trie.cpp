#include <raylib.h>
#include "../include/trie.h"
#include <iostream>


Trie::Trie(){
    
}

void Trie::init(){
    int buttonWidth = 120, buttonHeight = 30;
    int offsetX = 27, offsetY = 120 + 600 - 6*buttonHeight - 5 * 5;
    createButton  = ButtonText(offsetX, offsetY, buttonWidth, buttonHeight, "Create Random");
    searchButton = ButtonText(offsetX, offsetY + buttonHeight + 5, buttonWidth, buttonHeight, "Search");
    addButton = ButtonText(offsetX, offsetY + 2*buttonHeight + 10, buttonWidth, buttonHeight, "Insert");    
    deleteButton = ButtonText(offsetX, offsetY + 3*buttonHeight + 15, buttonWidth, buttonHeight, "Delete");
    fileButton = ButtonText(offsetX, offsetY + 4*buttonHeight + 20, buttonWidth, buttonHeight, "File");
    clearButton = ButtonText(offsetX, offsetY + 5*buttonHeight + 25, buttonWidth, buttonHeight, "Clear");

    remote.resize(7);
    remote[0] = {{350,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/pre.png"),RAYWHITE,1};    
    remote[1] = {{remote[0].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/undo.png"),RAYWHITE,1};
    remote[2] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/play.png"),RAYWHITE,1};
    remote[3] = {{remote[2].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/redo.png"),RAYWHITE,1};
    remote[4] = {{remote[3].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/suff.png"),RAYWHITE,1};
    remote[5] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/pause.png"),RAYWHITE,1};
    remote[6] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/repeat.png"),RAYWHITE,1};
    
    animationState = PAUSE;
    timeAnimation = 0.0f;
    curAnimation = 0;
    Animation.clear();

    inputActive = false;    // Flag to check if input is active
    inputType = 0;         // Type of input (0: None, 1: Create, 2: Search, 3: Insert, 4: Delete)

    lineCount = 8;
    isPseudocodeVisible = false;
    togglePseudocodeButton = { (float)screenWidth-20, 490, 20, (float)lineCount * 30};

    root = nullptr;
}

void Trie::updatePseudocodeOn() {
    Vector2 mousePosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePosition, togglePseudocodeButton)) {
            isPseudocodeVisible = !isPseudocodeVisible; 
        }
    }
}

void Trie::drawPseudocode() {

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

float Trie::generatePosition(TrieNode* u, float t, float f, float h, float delh, float r) {
    if (u == nullptr) return t;

    float g = t;
    float mid = -1;
    float totalChildPos = 0; // Tổng vị trí của các nút con
    int childCount = 0;      // Số lượng nút con

    for (TrieNode* v : u->children) {
        if (v == nullptr) continue;
        g = generatePosition(v, g, f, h + delh, delh, r);
        totalChildPos += v->node->position.x; // Cộng dồn vị trí x của các nút con
        childCount++;
    }

    // Nếu có nút con, tính vị trí trung bình
    if (childCount > 0) {
        mid = totalChildPos / childCount;
    } else {
        mid = (std::max(g - f - 2 * r, t) + t) / 2.0f; // Nếu không có nút con, tính như cũ
    }

    u->node->position = {mid, h};
    u->node->radius = r;

    return std::max(g, mid + 3 * r);
}

void Trie::adjustPosition(){
    float r = radius;
    float dis = 30;
    float k = screenWidth;
    for(int i = r; i >= 5; --i){
        dis -= 3;
        r -= 0.5;
        k = screenWidth - (generatePosition(root, 0, dis, 150, 70, r) - dis - 2*r);
        if(k > 30) break;
    }
    generatePosition(root, k/2.0f, dis, 150, 70, r);
}

void Trie::drawTrie(TrieNode* u){
    if(u == nullptr) return;
    u->node->draw();
    for(int i = 0; i < 26; ++i){
        if(u->children[i] != nullptr){
            DrawConnection(u->node->position, u->children[i]->node->position, true, BLACK, 4.0f, u->node->radius, u->children[i]->node->radius);
            drawTrie(u->children[i]);
        }
    }
}

void Trie::draw(){
    handleRemote();

    drawTrie(root);

    drawButtons();
}

void Trie::createRandomTrie(){
    clear();
}

void Trie::insert(std::string word){
    if(word.empty()) return;
    // std::cerr << word << ' ' << '\n';
    if(root == nullptr){
        root = new TrieNode();
        root->node = new Node("", {130 + 645, 120}, BLACK, radius, customFont, 20.0f);
    }
    TrieNode* cur = root;
    for(char ch : word){
        int index = ch - 'A';
        if(cur->children[index] == nullptr){
            cur->children[index] = new TrieNode();
            cur->children[index]->node = new Node(std::string(1, ch), {0, 0}, BLACK, radius, customFont, 20.0f);
            cur->children[index]->parent = cur;
        }
        cur = cur->children[index];
    }
    cur->isEndOfWord = true;
    listStr.push_back(word);
    adjustPosition();
}

void Trie::search(std::string word){
    if(word.empty()) return;
    // std::cerr << word << ' ' << '\n';
    if(root == nullptr) return;
    TrieNode* cur = root;
    for(char ch : word){
        int index = ch - 'A';
        if(cur->children[index] == nullptr) return;
        cur = cur->children[index];
    }
    if(cur->isEndOfWord) {
        std::cerr << "Found: " << word << '\n';
    } else {
        std::cerr << "Not Found: " << word << '\n';
    }
}

void Trie::remove(std::string word){
    if(word.empty()) return;
    // std::cerr << word << ' ' << '\n';
    if(root == nullptr) return;
    TrieNode* cur = root;
    for(char ch : word){
        int index = ch - 'A';
        if(cur->children[index] == nullptr) return;
        cur = cur->children[index];
    }
    if(cur->isEndOfWord) {
        cur->isEndOfWord = false;
        listStr.erase(std::remove(listStr.begin(), listStr.end(), word), listStr.end());
        adjustPosition();
    } else {
        std::cerr << "Not Found: " << word << '\n';
    }
}

void Trie::deleteNode(TrieNode* node){
    if (node == nullptr) return;
    for(int i = 0; i < 26; ++i){
        if(node->children[i] != nullptr) deleteNode(node->children[i]);
    }
    // delete node->parent;
    delete node->node;
    delete node;         
}

void Trie::clear(){
    listStr.clear();
    deleteNode(root);
    root = nullptr;
    if(root == nullptr) std::cerr << "YES\n";
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
}

void Trie::createRandomString(int len) {
    for(int i = 0; i < len; ++i){
        inputBuffer[i] = (char)(Rand(0, 25) + 'A'); // Generate a random uppercase letter
    }
    inputBuffer[len] = '\0'; // Null-terminate the string
}

void Trie::handleEvents(){
    Vector2 mousePosition = GetMousePosition(); // Get the mouse position

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        // Check if the create button is clicked
        if (createButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 1; // Set input type to create
        }
        else if (searchButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 2; // Set input type to search
            createRandomString(Rand(3, 7));
        }

        // Check if the insert button is clicked
        else if (addButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 3; // Set input type to insert
            createRandomString(Rand(3, 7));

        }

        // Check if the delete button is clicked
        else if (deleteButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 4; // Set input type to delete
            if(!listStr.empty()) strcpy(inputBuffer, listStr[Rand(0, listStr.size()-1)].c_str()); // Randomly select a string from the list
            else createRandomString(Rand(3, 7)); // If the list is empty, generate a random string
        }

        //Check if the file button is clicked
        else if (fileButton.IsHovered(mousePosition)) {
            loadFile();
        }

        else if (clearButton.IsHovered(mousePosition)) {
            clear(); // Clear the hash table
        }
        
        // If no button is clicked, deactivate input
        else if (inputActive) {
            inputActive = false; // Deactivate input if no button is clicked
            inputType = 0; // Reset input type
        }
    }
    
    // Handle keyboard input
    if (inputActive) {
        int key = GetCharPressed(); 
        while (key > 0) {
            // Check if the key is a valid character from 'a' to 'z' or 'A' to 'Z'
            if (((int)('A') <= key && key <= (int)('Z')) || ((int)('a') <= key && key <= (int)('z'))) {
                int len = strlen(inputBuffer);
                if(len >= 20) break;
                if((int)('a') <= key && key <= (int)('z')) key -= 32; // Convert lowercase to uppercase
                inputBuffer[len] = (char)key;
                inputBuffer[len + 1] = '\0';
            }
            key = GetCharPressed();
        }

        if(IsKeyPressed(KEY_BACKSPACE)) { 
            int len = strlen(inputBuffer);
            if (len > 0) inputBuffer[len - 1] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER)) {
            // std::cerr << inputBuffer << ' ' << value << '\n';
            switch (inputType) {
                case 1: createRandomTrie(); break;
                case 2: search(inputBuffer); break; 
                case 3: insert(inputBuffer); break;
                case 4: remove(inputBuffer); break; 
            }
            inputActive = false; 
            inputType = 0;
        }
    }
}

void Trie::drawButtons(){
    updatePseudocodeOn();
    addButton.Update();
    searchButton.Update();
    deleteButton.Update();
    createButton.Update();
    fileButton.Update();
    clearButton.Update();

    addButton.Draw();
    searchButton.Draw();
    deleteButton.Draw();
    createButton.Draw();
    fileButton.Draw();
    clearButton.Draw();

    DrawRectangleRec(togglePseudocodeButton, GREEN);
    if (isPseudocodeVisible) {
        // DrawRectangleLinesEx(togglePseudocodeButton, 2, BLACK);
        DrawTextEx(customFont, ">", { togglePseudocodeButton.x + (togglePseudocodeButton.width - MeasureText(">", 20)) / 2, togglePseudocodeButton.y + (togglePseudocodeButton.height - 20) / 2 }, 20, 2, WHITE);
    } else {
        // DrawRectangleLinesEx(togglePseudocodeButton, 2, BLACK);
        DrawTextEx(customFont, "<", { togglePseudocodeButton.x + (togglePseudocodeButton.width - MeasureText("<", 20)) / 2, togglePseudocodeButton.y + (togglePseudocodeButton.height - 20) / 2 }, 20, 2, WHITE);
    }

    if (inputActive) {
        if (inputType == 2){ // Search
            DrawRectangle(searchButton.bounds.x + searchButton.bounds.width + 10, searchButton.bounds.y, 150, searchButton.bounds.height, PINK); // Draw input box
            DrawText("S = ", searchButton.bounds.x + searchButton.bounds.width + 15, searchButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputBuffer, searchButton.bounds.x + searchButton.bounds.width + 55, searchButton.bounds.y + 5, 20, BLACK); // Draw input text
            if(static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", searchButton.bounds.x + searchButton.bounds.width + 55 + MeasureText(inputBuffer, 20), searchButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }
        }
        else if (inputType == 3){ // Insert
            DrawRectangle(addButton.bounds.x + addButton.bounds.width + 10, addButton.bounds.y, 150, addButton.bounds.height, PINK); // Draw input box
            DrawText("S = ", addButton.bounds.x + addButton.bounds.width + 15, addButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputBuffer, addButton.bounds.x + addButton.bounds.width + 55, addButton.bounds.y + 5, 20, BLACK); // Draw input text
            if(static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", addButton.bounds.x + addButton.bounds.width + 55 + MeasureText(inputBuffer, 20), addButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }
        }
        else if (inputType == 4){ // Delete
            DrawRectangle(deleteButton.bounds.x + deleteButton.bounds.width + 10, deleteButton.bounds.y, 150, deleteButton.bounds.height, PINK); // Draw input box
            DrawText("S = ", deleteButton.bounds.x + deleteButton.bounds.width + 15, deleteButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputBuffer, deleteButton.bounds.x + deleteButton.bounds.width + 55, deleteButton.bounds.y + 5, 20, BLACK); // Draw input text
            if(static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", deleteButton.bounds.x + deleteButton.bounds.width + 55 + MeasureText(inputBuffer, 20), deleteButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }
        }
    }
}

Trie::~Trie(){

}