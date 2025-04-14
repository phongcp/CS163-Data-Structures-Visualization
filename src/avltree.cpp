#include "../include/avltree.h"

AVLTree::AVLTree() {
    // Constructor implementation
    animationState = PAUSE;
    lineCount = 6;
    isPseudocodeVisible = false;
    togglePseudocodeButton = { (float)screenWidth - 20, 550, 20, (float)lineCount * 30 };
}


void AVLTree::init(){
    int buttonWidth = 120, buttonHeight = 30;
    int offsetX = 27, offsetY = 120 + 600 - 6*buttonHeight - 5 * 5;
    createButton  = ButtonText(offsetX, offsetY, buttonWidth, buttonHeight, "Create");
    searchButton = ButtonText(offsetX, offsetY + buttonHeight + 5, buttonWidth, buttonHeight, "Search");
    addButton = ButtonText(offsetX, offsetY + 2*buttonHeight + 10, buttonWidth, buttonHeight, "Insert");    
    deleteButton = ButtonText(offsetX, offsetY + 3*buttonHeight + 15, buttonWidth, buttonHeight, "Delete");
    fileButton = ButtonText(offsetX, offsetY + 4*buttonHeight + 20, buttonWidth, buttonHeight, "File");
    clearButton = ButtonText(offsetX, offsetY + 5*buttonHeight + 25, buttonWidth, buttonHeight, "Clear");

    // 6 * 30

    remote.resize(7);
    remote[0] = {{350,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/pre.png"),RAYWHITE,1};    
    remote[1] = {{remote[0].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/undo.png"),RAYWHITE,1};
    remote[2] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/play.png"),RAYWHITE,1};
    remote[3] = {{remote[2].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/redo.png"),RAYWHITE,1};
    remote[4] = {{remote[3].Postion.x + 69,747},{49,52},(Texture2D)LoadTexture("../assets/in_app/suff.png"),RAYWHITE,1};
    remote[5] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/pause.png"),RAYWHITE,1};
    remote[6] = {{remote[1].Postion.x + 69,751},{44,50},(Texture2D)LoadTexture("../assets/in_app/repeat.png"),RAYWHITE,1};
    

    animationState = PAUSE;

    inputActive = false;

    lineCount = 6;
    isPseudocodeVisible = false;
    togglePseudocodeButton = { (float)screenWidth-20, 550, 20, (float)lineCount * 30};
}


//---------------------Pseudocode---------------------//
void AVLTree::updatePseudocodeOn() {
    Vector2 mousePosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePosition, togglePseudocodeButton)) {
            isPseudocodeVisible = !isPseudocodeVisible; 
        }
    }
}

void AVLTree::drawPseudocode(const char** pseudocode, int codeLine1, int codeLine2) {
    float startX = (float)screenWidth - 400 - 5 - 30;
    float startY = 550;

    float boxWidth = 410;
    float boxHeight = 30;

    Rectangle pseudocodeBox = { 
        startX, 
        startY, 
        410.0f, 
        30.0f * (float)lineCount 
    };
    DrawRectangleRec(pseudocodeBox, LIGHTGRAY);
    if(pseudocode == nullptr) return;

    for(int i = 0; i < lineCount; ++i){
        // float newX = startX;
        // float newY = startY + i * boxHeight;
        // Rectangle textBox = {newX, newY + 5, boxWidth, boxHeight - 9};
        // Color backgroundColor = (i == codeLine1 || i == codeLine2) ? GREEN : LIGHTGRAY;
        // DrawRectangleRec(textBox, backgroundColor);

        // Color textColor = (i == codeLine1 || i == codeLine2) ? WHITE : BLACK;
        // DrawTextEx(fontPseudocode, pseudocode[i], { newX + 5, newY + 5 }, 20, 2, textColor);
    }
}

void AVLTree::handleRemote(){
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
                    // curAnimation = Animation.size() - 1;
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

void AVLTree::draw(){
    handleRemote();
    
    drawButtons();
}

void AVLTree::loadFile(){

}

void AVLTree::clear(){

}

void AVLTree::handleEvents(){
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
        }

        // Check if the insert button is clicked
        else if (addButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 3; // Set input type to insert

        }

        // Check if the delete button is clicked
        else if (deleteButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 4; // Set input type to delete
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
                // case 1: createRandomTrie(); break;
                // case 2: search(inputBuffer); break; 
                // case 3: insert(inputBuffer); break;
                // case 4: remove(inputBuffer); break; 
            }
            inputActive = false; 
            inputType = 0;
        }
    }
}

void AVLTree::drawButtons() {
    // DrawRectangle(0, 155, 350, 600, LIGHTGRAY); // Draw button background
    // DrawLine(175, 155, 175, 155 + 600, DARKGRAY); // Draw line between menu and button
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


    // Draw the input buffer if input is active
    // if (inputActive) {
    //     DrawRectangle(50, 720 - 4*30 - 5, 120, 30, LIGHTGRAY); // Draw input box
    //     DrawText(inputBuffer, 55, 720 - 4*30 - 5 + 5, 20, BLACK); // Draw input text
    // }

    DrawRectangleRec(togglePseudocodeButton, GREEN);
    if (isPseudocodeVisible) {
        // DrawRectangleLinesEx(togglePseudocodeButton, 2, BLACK);
        DrawTextEx(customFont, ">", { togglePseudocodeButton.x + (togglePseudocodeButton.width - MeasureText(">", 20)) / 2, togglePseudocodeButton.y + (togglePseudocodeButton.height - 20) / 2 }, 20, 2, WHITE);
    } else {
        // DrawRectangleLinesEx(togglePseudocodeButton, 2, BLACK);
        DrawTextEx(customFont, "<", { togglePseudocodeButton.x + (togglePseudocodeButton.width - MeasureText("<", 20)) / 2, togglePseudocodeButton.y + (togglePseudocodeButton.height - 20) / 2 }, 20, 2, WHITE);
    }

    if (inputActive) {
        if (inputType == 1){  //Create
            DrawRectangle(createButton.bounds.x + createButton.bounds.width + 10, createButton.bounds.y, 120, createButton.bounds.height, PINK); // Draw input box
            DrawText("N = ", createButton.bounds.x + createButton.bounds.width + 15, createButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputBuffer, createButton.bounds.x + createButton.bounds.width + 55, createButton.bounds.y + 5, 20, BLACK); // Draw input text
            if (inputBuffer && static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", createButton.bounds.x + createButton.bounds.width + 55 + MeasureText(inputBuffer, 20), createButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }
        }
        else if (inputType == 2){ // Search
            DrawRectangle(searchButton.bounds.x + searchButton.bounds.width + 10, searchButton.bounds.y, 150, searchButton.bounds.height, PINK); // Draw input box
            DrawText("V = ", searchButton.bounds.x + searchButton.bounds.width + 15, searchButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputBuffer, searchButton.bounds.x + searchButton.bounds.width + 55, searchButton.bounds.y + 5, 20, BLACK); // Draw input text
            if(static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", searchButton.bounds.x + searchButton.bounds.width + 55 + MeasureText(inputBuffer, 20), searchButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }
        }
        else if (inputType == 3){ // Insert
            DrawRectangle(addButton.bounds.x + addButton.bounds.width + 10, addButton.bounds.y, 150, addButton.bounds.height, PINK); // Draw input box
            DrawText("V = ", addButton.bounds.x + addButton.bounds.width + 15, addButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputBuffer, addButton.bounds.x + addButton.bounds.width + 55, addButton.bounds.y + 5, 20, BLACK); // Draw input text
            if(static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", addButton.bounds.x + addButton.bounds.width + 55 + MeasureText(inputBuffer, 20), addButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }
        }
        else if (inputType == 4){ // Delete
            DrawRectangle(deleteButton.bounds.x + deleteButton.bounds.width + 10, deleteButton.bounds.y, 150, deleteButton.bounds.height, PINK); // Draw input box
            DrawText("V = ", deleteButton.bounds.x + deleteButton.bounds.width + 15, deleteButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputBuffer, deleteButton.bounds.x + deleteButton.bounds.width + 55, deleteButton.bounds.y + 5, 20, BLACK); // Draw input text
            if(static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", deleteButton.bounds.x + deleteButton.bounds.width + 55 + MeasureText(inputBuffer, 20), deleteButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }
        }
    }
}

AVLTree::~AVLTree() {
    // Destructor implementation
    // Clean up resources if needed
}