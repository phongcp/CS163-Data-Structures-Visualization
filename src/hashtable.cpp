#include "../include/hashtable.h"
#include "raylib.h" // Include raylib for visualization
#include <iostream>

// Constructor
HashTable::HashTable() : capacity(0),
    size(0),
    inputActive(false),
    inputType(0){
    memset(inputBuffer, 0, sizeof(inputBuffer)); // Clear the input buffer
}

// Initialize the hash table
void HashTable::init(){
    int buttonWidth = 120, buttonHeight = 30;
    int offsetX = 27, offsetY = 120 + 600 - 6*buttonHeight - 5 * 5;
    createButton  = ButtonText(offsetX, offsetY, buttonWidth, buttonHeight, "Create");
    sizeInputBox = {createButton.bounds.x + createButton.bounds.width + 10 + 125, createButton.bounds.y, 120, createButton.bounds.height};
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

    lineCount = 6;
    isPseudocodeVisible = false;
    togglePseudocodeButton = { (float)screenWidth-20, 550, 20, (float)lineCount * 30};
}

void HashTable::updatePseudocodeOn() {
    Vector2 mousePosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePosition, togglePseudocodeButton)) {
            isPseudocodeVisible = !isPseudocodeVisible; 
        }
    }
}

void HashTable::drawPseudocode(const char** pseudocode, int codeLine1, int codeLine2) {
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
        float newX = startX;
        float newY = startY + i * boxHeight;
        Rectangle textBox = {newX, newY + 5, boxWidth, boxHeight - 9};
        Color backgroundColor = (i == codeLine1 || i == codeLine2) ? GREEN : LIGHTGRAY;
        DrawRectangleRec(textBox, backgroundColor);

        Color textColor = (i == codeLine1 || i == codeLine2) ? WHITE : BLACK;
        DrawTextEx(fontPseudocode, pseudocode[i], { newX + 5, newY + 5 }, 20, 2, textColor);
    }
}

void HashTable::handleRemote(){
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

void HashTable::drawHashTable(Snapshot curShot, float opacity){
    // Draw pseudocode for the hash table
    if(isPseudocodeVisible){
        drawPseudocode(curShot.code, curShot.codeLine1, curShot.codeLine2); // Draw the pseudocode
    }    
    for(int i = 0; i < capacity; ++i){
        // std::cerr << curShot.table[i].data << '\n';
        // if(animationState == PAUSE)
        if (animationState == PAUSE) curShot.table[i].draw(1.0f);
        else if(curShot.table[i].isUpdate){
            curShot.table[i].draw(opacity); // Draw the current state of the hash table
        }
        else{
            curShot.table[i].draw(1.0f); // Draw the current state of the hash table
        }
    }
}

void HashTable::draw(){
    handleRemote();
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
        drawHashTable(Animation[curAnimation], opacity); // Draw the current animation frame
    }
    else{
        if(isPseudocodeVisible){
            drawPseudocode(nullptr, -1, -1); // Draw the pseudocode
        }
    }
}

Vector2 HashTable::generatePosition(int index){
    int row = index / CELLS_PER_ROW; // Calculate the row number
    int col = index % CELLS_PER_ROW; // Calculate the column number
    return {OFFSET_X + col * CELL_SPACING_X, OFFSET_Y + row * CELL_SPACING_Y}; // Return the position of the cell
}

void HashTable::createRandomTable(int newCapacity, int newSize){
    Animation.clear(); // Clear the animation history
    curAnimation = 0; // Reset the current animation index
    timeAnimation = 0.0f; // Reset the current time
    animationState = PLAY; // Set the animation state to pause

    capacity = newCapacity; // Set the new capacity
    size = newSize;
    if(size > capacity) return;
    hashTable.assign(capacity, NodeHashTable(-1)); // Resize the hash table to the new capacity

    for(int i = 0; i < size; i++){
        int num = rand() % 100; // Generate a random number between 0 and 99
        insertValueToHashTable(num); // Insert the number into the hash table
    }

    for(int i = 0; i < capacity; i++){
        hashTable[i] = NodeHashTable(hashTable[i].data,
                                    generatePosition(i), // Generate the position of the cell
                                    LIGHTGRAY, // Set the color of the cell
                                    DARKGRAY, // Set the border color of the cell
                                    FULL_RADIUS,
                                    customFont, 
                                    20.0f,
                                    true);
    }
    Animation.push_back(Snapshot(hashTable)); // Add the current state of the hash table to the animation history
    for(int i = 0; i < capacity; ++i){
        hashTable[i].isUpdate = false; // Mark the cell as not updated
    }
    Animation.push_back(Snapshot(hashTable));
}

void HashTable::insertValueToHashTable(int num){
    int index = num % capacity; // Hash function to find the index
    while (hashTable[index].data != -1) { // Find an empty slot by linear probing
        index = (index + 1) % capacity;
    }
    hashTable[index].data = num; // Insert the number into the hash table
}

void HashTable::insert(int num){
    if(capacity == 0) return;
    Animation.clear(); // Clear the animation history
    curAnimation = 0; // Reset the current animation index
    timeAnimation = 0.0f; // Reset the current time    
    animationState = PLAY;

    for(int i = 0; i < capacity; ++i){
        hashTable[i] = NodeHashTable(hashTable[i].data, generatePosition(i), LIGHTGRAY, DARKGRAY, FULL_RADIUS, customFont, 20.0f, false);
    }

    if(size + 1 >= capacity){
        Animation.push_back(Snapshot(hashTable, insertValueCode, 0, 0));
        return;
    }
    else Animation.push_back(Snapshot(hashTable, insertValueCode, 0, 1));

    int index = num % capacity; // Hash function to find the index
    int startIndex = index; // Store the starting index for collision detection
    while (hashTable[index].data != EMPTY && hashTable[index].data != DELETED) { // Find an empty slot by linear probing
        // Add the animation for collision
        hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), YELLOW, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
        Animation.push_back(Snapshot(hashTable, insertValueCode, 2, 3)); // Add the current state of the hash table to the animation history
        hashTable[index].isUpdate = false; // Mark the cell as not updated

        hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), LIGHTGRAY, YELLOW, FULL_RADIUS, customFont, 20.0f, true);
        Animation.push_back(Snapshot(hashTable, insertValueCode, 2, 3));
        hashTable[index].isUpdate = false; 

        index = (index + 1) % capacity;
    }
    ++size;
    hashTable[index].data = num; // Insert the number into the hash table
    hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), SKYBLUE, DARKBLUE, FULL_RADIUS, customFont, 20.0f, true);
    Animation.push_back(Snapshot(hashTable, insertValueCode, 4, 4)); // Add the current state of the hash table to the animation history
    hashTable[index].isUpdate = false; // Mark the cell as not updated
}

void HashTable::search(int num){
    if(capacity == 0) return;
    Animation.clear();
    curAnimation = 0;
    timeAnimation = 0.0f;
    animationState = PLAY;

    for(int i = 0; i < capacity; ++i){
        hashTable[i] = NodeHashTable(hashTable[i].data, generatePosition(i), LIGHTGRAY, DARKGRAY, FULL_RADIUS, customFont, 20.0f, false);
    }
    Animation.push_back(Snapshot(hashTable, searchValueCode, 0, 0)); 
    int index = num % capacity; // Hash function to find the index
    int startIndex = index; // Store the starting index for collision detection
    while(hashTable[index].data != EMPTY) { // Find the number in the hash table
        if(hashTable[index].data == num){ // If the number is found
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), GREEN, DARKGREEN, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, searchValueCode, 1, 2)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated
            return;
        }
        else{ // If the number is not found
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), YELLOW, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, searchValueCode, 1, 3)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated
    
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), LIGHTGRAY, YELLOW, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, searchValueCode, 1, 3));
            hashTable[index].isUpdate = false; 
        }
        index = (index + 1) % capacity; // Move to the next index
        if(index == startIndex) { // If we have looped through the entire table
            break;
        }
    }
    hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), YELLOW, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
    Animation.push_back(Snapshot(hashTable, searchValueCode, 4, 4)); // Add the current state of the hash table to the animation history
    hashTable[index].isUpdate = false; // Mark the cell as not updated

    hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), LIGHTGRAY, YELLOW, FULL_RADIUS, customFont, 20.0f, true);
    Animation.push_back(Snapshot(hashTable, searchValueCode, 4, 4)); // Add the current state of the hash table to the animation history
    hashTable[index].isUpdate = false; // Mark the cell as not updated
}

void HashTable::remove(int num){
    if(capacity == 0) return;
    Animation.clear();
    curAnimation = 0;
    timeAnimation = 0.0f;
    animationState = PLAY;

    for(int i = 0; i < capacity; ++i){
        hashTable[i] = NodeHashTable(hashTable[i].data, generatePosition(i), LIGHTGRAY, DARKGRAY, FULL_RADIUS, customFont, 20.0f, false);
    }
    Animation.push_back(Snapshot(hashTable, deleteValueCode, 0, 0));
    int index = num % capacity; // Hash function to find the index
    int startIndex = index; // Store the starting index for collision detection
    while(hashTable[index].data != EMPTY){
        if(hashTable[index].data == num){
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), RED, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 1, 2)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated

            hashTable[index].data = DELETED; // Mark the cell as deleted
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), RED, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 1, 2)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated
            return;
        }
        else{
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), YELLOW, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 1, 3)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated
    
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), LIGHTGRAY, YELLOW, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 1, 3));
            hashTable[index].isUpdate = false; 
        }
        index = (index + 1) % capacity; 
        if(index == startIndex){
            break;
        }
    }
    hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), YELLOW, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
    Animation.push_back(Snapshot(hashTable, deleteValueCode, 4, 4)); // Add the current state of the hash table to the animation history
    hashTable[index].isUpdate = false; // Mark the cell as not updated
}

void HashTable::clear(){
    Animation.clear();
    curAnimation = 0;
    timeAnimation = 0.0f;
    animationState = PAUSE;
    hashTable.clear(); // Clear the hash table    
}

void HashTable::loadFile(){
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

    int n, k;
    fscanf(file, "%d %d", &n, &k); // Read the size and number of elements
    capacity = n; 
    size = k;
    hashTable.assign(capacity, NodeHashTable(-1)); 
    for(int i = 0; i < size; i++){ // Read the elements from the file
        int num;
        fscanf(file, "%d", &num); 
        insertValueToHashTable(num); 
    }
    for(int i = 0; i < capacity; i++){
        hashTable[i] = NodeHashTable(hashTable[i].data,
                                    generatePosition(i),
                                    LIGHTGRAY, 
                                    DARKGRAY, 
                                    FULL_RADIUS,
                                    customFont, 
                                    20.0f,
                                    true);
    }
    Animation.push_back(Snapshot(hashTable));
}

void HashTable::handleEvents(){
    Vector2 mousePosition = GetMousePosition(); // Get the mouse position

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        int preInputType = inputType;
        // Check if the create button is clicked
        if (createButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 1; // Set input type to create
            // memset(inputCapacityBuffer, 0, sizeof(inputCapacityBuffer)); // Clear the input buffer
            // memset(inputSizeBuffer, 0, sizeof(inputSizeBuffer)); // Clear the input size buffer
        } 
        
        // Check if the search button is clicked
        else if (searchButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 2; // Set input type to search
            inputBuffer[0] = Rand(0, 9) + '0'; inputBuffer[1] = Rand(0, 9) + '0';
            if(inputBuffer[0] == '0') inputBuffer[0] = inputBuffer[1], inputBuffer[1] = '\0';
            else inputBuffer[2] = '\0';
        }

        // Check if the insert button is clicked
        else if (addButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 3; // Set input type to insert
            inputBuffer[0] = Rand(0, 9) + '0'; inputBuffer[1] = Rand(0, 9) + '0';
            if(inputBuffer[0] == '0') inputBuffer[0] = inputBuffer[1], inputBuffer[1] = '\0';
            else inputBuffer[2] = '\0';
        }

        // Check if the delete button is clicked
        else if (deleteButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 4; // Set input type to delete
            inputBuffer[0] = Rand(0, 9) + '0'; inputBuffer[1] = Rand(0, 9) + '0';
            if(inputBuffer[0] == '0') inputBuffer[0] = inputBuffer[1], inputBuffer[1] = '\0';
            else inputBuffer[2] = '\0';
        }

        //Check if the file button is clicked
        else if (fileButton.IsHovered(mousePosition)) {
            loadFile();
        }

        else if (clearButton.IsHovered(mousePosition)) {
            clear(); // Clear the hash table
        }
        
        // If no button is clicked, deactivate input
        // int offsetX = 27, offsetY = 120 + 600 - 6*buttonHeight - 5 * 5;
        // if (inputActive && preInputType == inputType) {
        //     inputActive = false; // Deactivate input if no button is clicked
        //     inputType = 0; // Reset input type
        // }
    }
    
    // Handle keyboard input
    if (inputActive) {
        if(inputType == 1){
            if(isGetInputCapacity){
                int key = GetCharPressed(); // Get the pressed key
                while (key > 0) {
                    if (key >= 48 && key <= 57) { // Check if the key is a valid character
                        int len = strlen(inputCapacityBuffer);
                        if(len == 2) break;
                        inputCapacityBuffer[len] = (char)key;
                        inputCapacityBuffer[len + 1] = '\0'; // Append the character to the input buffer
                    }
                    key = GetCharPressed(); // Get the next pressed key
                }
                if (IsKeyPressed(KEY_BACKSPACE)) { // Handle backspace key
                    int len = strlen(inputCapacityBuffer); // Get the length of the input buffer
                    if (len > 0) inputCapacityBuffer[len - 1] = '\0'; // Remove the last character from the input buffer
                }
            }
            else if(isGetInputSize){
                int key = GetCharPressed(); 
                while (key > 0) {
                    if (key >= 48 && key <= 57) { // Check if the key is a valid character
                        int len = strlen(inputSizeBuffer);
                        if(len == 2) break;
                        inputSizeBuffer[len] = (char)key;
                        inputSizeBuffer[len + 1] = '\0'; // Append the character to the input buffer
                    }
                    key = GetCharPressed(); // Get the next pressed key
                }
                if (IsKeyPressed(KEY_BACKSPACE)) { // Handle backspace key
                    int len = strlen(inputSizeBuffer); // Get the length of the input buffer
                    if (len > 0) inputSizeBuffer[len - 1] = '\0'; // Remove the last character from the input buffer
                }
            }
            if (IsKeyPressed(KEY_ENTER)) {
                int newCapacity = std::atoi(inputCapacityBuffer); 
                int newSize;
                if(inputSizeBuffer[0] == '\0') newSize = Rand(0, newCapacity/2); // If size is not set, set it to 0
                else newSize = std::atoi(inputSizeBuffer); 
                createRandomTable(newCapacity, newSize);
                isGetInputCapacity = false; 
                inputActive = false;
                inputType = 0;
            }
        }
        else{
            int value = 0;
            int key = GetCharPressed(); // Get the pressed key
            while (key > 0) {
                if (key >= 48 && key <= 57) { // Check if the key is a valid character
                    int len = strlen(inputBuffer);
                    if(len >= 5) break;
                    inputBuffer[len] = (char)key;
                    inputBuffer[len + 1] = '\0'; // Append the character to the input buffer
                }
                key = GetCharPressed(); // Get the next pressed key
            }

            if(IsKeyPressed(KEY_BACKSPACE)) { // Handle backspace key
                int len = strlen(inputBuffer); // Get the length of the input buffer
                if (len > 0) inputBuffer[len - 1] = '\0'; // Remove the last character from the input buffer
            }

            // Handle Enter key for different input types
            if (IsKeyPressed(KEY_ENTER)) {
                value = std::atoi(inputBuffer); // Convert the input buffer to an integer
                // std::cerr << inputBuffer << ' ' << value << '\n';
                switch (inputType) {
                    case 2: search(value); break; // Search for a value by key
                    case 3: insert(value); break; // Insert a new key-value pair
                    case 4: remove(value); break; // Delete a key-value pair
                }
                inputActive = false; // Deactivate input after processing
                inputType = 0; // Reset input type
            }
        }
    }
}

void HashTable::drawButtons() {
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
        if(inputType == 1){ // Create
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if (CheckCollisionPointRec(GetMousePosition(), {createButton.bounds.x + createButton.bounds.width + 10, createButton.bounds.y, 120, createButton.bounds.height})){
                    isGetInputCapacity = true;
                    isGetInputSize = false;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), sizeInputBox)){
                    isGetInputCapacity = false;
                    isGetInputSize = true;
                }
            } 
            // get value for capacity
            DrawRectangle(createButton.bounds.x + createButton.bounds.width + 10, createButton.bounds.y, 120, createButton.bounds.height, PINK); // Draw input box
            DrawText("N = ", createButton.bounds.x + createButton.bounds.width + 15, createButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputCapacityBuffer, createButton.bounds.x + createButton.bounds.width + 55, createButton.bounds.y + 5, 20, BLACK); // Draw input text
            if (isGetInputCapacity && static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", createButton.bounds.x + createButton.bounds.width + 55 + MeasureText(inputCapacityBuffer, 20), createButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }            
            // get value for size
            DrawRectangleRec(sizeInputBox, PINK); 
            DrawText("K = ", sizeInputBox.x + 15, sizeInputBox.y + 5, 20, BLACK);
            DrawText(inputSizeBuffer, sizeInputBox.x + 55, sizeInputBox.y + 5, 20, BLACK); // Draw input text
            if(isGetInputSize && static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", sizeInputBox.x + 55 + MeasureText(inputSizeBuffer, 20), sizeInputBox.y + 5, 20, BLACK); // Draw cursor
            }
        }
        else if (inputType == 2){ // Search
            DrawRectangle(searchButton.bounds.x + searchButton.bounds.width + 10, searchButton.bounds.y, 120, searchButton.bounds.height, PINK); // Draw input box
            DrawText("V = ", searchButton.bounds.x + searchButton.bounds.width + 15, searchButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputBuffer, searchButton.bounds.x + searchButton.bounds.width + 55, searchButton.bounds.y + 5, 20, BLACK); // Draw input text
            if(static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", searchButton.bounds.x + searchButton.bounds.width + 55 + MeasureText(inputBuffer, 20), searchButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }
        }
        else if (inputType == 3){ // Insert
            DrawRectangle(addButton.bounds.x + addButton.bounds.width + 10, addButton.bounds.y, 120, addButton.bounds.height, PINK); // Draw input box
            DrawText("V = ", addButton.bounds.x + addButton.bounds.width + 15, addButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputBuffer, addButton.bounds.x + addButton.bounds.width + 55, addButton.bounds.y + 5, 20, BLACK); // Draw input text
            if(static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", addButton.bounds.x + addButton.bounds.width + 55 + MeasureText(inputBuffer, 20), addButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }
        }
        else if (inputType == 4){ // Delete
            DrawRectangle(deleteButton.bounds.x + deleteButton.bounds.width + 10, deleteButton.bounds.y, 120, deleteButton.bounds.height, PINK); // Draw input box
            DrawText("V = ", deleteButton.bounds.x + deleteButton.bounds.width + 15, deleteButton.bounds.y + 5, 20, BLACK); // Draw input text
            DrawText(inputBuffer, deleteButton.bounds.x + deleteButton.bounds.width + 55, deleteButton.bounds.y + 5, 20, BLACK); // Draw input text
            if(static_cast<int>(GetTime() * 2) % 2 == 0){
                DrawText("|", deleteButton.bounds.x + deleteButton.bounds.width + 55 + MeasureText(inputBuffer, 20), deleteButton.bounds.y + 5, 20, BLACK); // Draw cursor
            }
        }

        // DrawRectangle(175, 720 - 4*30 - 5, 120, 30, PINK); // Draw input box
        // DrawRectangleLines(175, 720 - 4*30 - 5, 120, 30, BLACK); // Draw input box border        
        // DrawText(inputBuffer, 180, 720 - 4*30 - 5 + 5, 20, BLACK); // Draw input text
        // DrawTextEx(customFont, inputBuffer, { 180, 720 - 4*30 - 5 + 5 }, 20, 2, BLACK); // Draw input text with custom font
        
        // Sử dụng GetTime() để nhấp nháy con trỏ
        // if (static_cast<int>(GetTime() * 2) % 2 == 0) {
        //     DrawText("|" , 180 + MeasureText(inputBuffer, 20), 720 - 4*30 - 5 + 5, 20, BLACK); // Draw cursor
        // }
    }
}

// Destructor
HashTable::~HashTable() {
    // No dynamic memory to free
}