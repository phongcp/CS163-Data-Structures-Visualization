#include "../include/hashtable.h"
#include "raylib.h" // Include raylib for visualization
#include <iostream>

// Constructor
HashTable::HashTable() : capacity(0),
    size(0),
    inputActive(false),
    inputType(0){

}

// Initialize the hash table
void HashTable::init(){
    int buttonWidth = 120, buttonHeight = 30;
    int offsetX = 27, offsetY = 120 + 600 - 7*buttonHeight - 6 * 5;
    createButton  = ButtonText(offsetX, offsetY, buttonWidth, buttonHeight, "Create");
    sizeInputBox = {createButton.bounds.x + createButton.bounds.width + 10 + 125, createButton.bounds.y, 120, createButton.bounds.height};
    searchButton = ButtonText(offsetX, offsetY + buttonHeight + 5, buttonWidth, buttonHeight, "Search");
    addButton = ButtonText(offsetX, offsetY + 2*buttonHeight + 10, buttonWidth, buttonHeight, "Insert");    
    updateButton = ButtonText(offsetX, offsetY + 3*buttonHeight + 15, buttonWidth, buttonHeight, "Update");
    deleteButton = ButtonText(offsetX, offsetY + 4*buttonHeight + 20, buttonWidth, buttonHeight, "Delete");
    fileButton = ButtonText(offsetX, offsetY + 5*buttonHeight + 25, buttonWidth, buttonHeight, "File");
    clearButton = ButtonText(offsetX, offsetY + 6*buttonHeight + 30, buttonWidth, buttonHeight, "Clear");

    createNTextBox = InputTextBox({createButton.bounds.x + createButton.bounds.width + 10, createButton.bounds.y, 120, createButton.bounds.height}, "N = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    createKTextBox = InputTextBox({createButton.bounds.x + createButton.bounds.width + 10 + 125, createButton.bounds.y, 120, createButton.bounds.height}, "K = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    searchTextBox = InputTextBox({searchButton.bounds.x + searchButton.bounds.width + 10, searchButton.bounds.y, 120, searchButton.bounds.height}, "V = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    addTextBox = InputTextBox({addButton.bounds.x + addButton.bounds.width + 10, addButton.bounds.y, 120, addButton.bounds.height}, "V = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    updateUTextBox = InputTextBox({updateButton.bounds.x + updateButton.bounds.width + 10, updateButton.bounds.y, 120, updateButton.bounds.height}, "U = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    updateVTextBox = InputTextBox({updateButton.bounds.x + updateButton.bounds.width + 10 + 125, updateButton.bounds.y, 120, updateButton.bounds.height}, "V = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    deleteTextBox = InputTextBox({deleteButton.bounds.x + deleteButton.bounds.width + 10, deleteButton.bounds.y, 120, deleteButton.bounds.height}, "V = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    


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
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 2, 3)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated

            hashTable[index].data = DELETED; // Mark the cell as deleted
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), RED, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 2, 3)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated
            return;
        }
        else{
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), YELLOW, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 1, 4)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated
    
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), LIGHTGRAY, YELLOW, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 1, 4));
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

void HashTable::update(int u, int v){
    if(capacity == 0) return;
    Animation.clear();
    curAnimation = 0;
    timeAnimation = 0.0f;
    animationState = PLAY;

    // Delete the old value
    for(int i = 0; i < capacity; ++i){
        hashTable[i] = NodeHashTable(hashTable[i].data, generatePosition(i), LIGHTGRAY, DARKGRAY, FULL_RADIUS, customFont, 20.0f, false);
    }
    Animation.push_back(Snapshot(hashTable, deleteValueCode, 0, 0));
    int index = u % capacity; // Hash function to find the index
    int startIndex = index; // Store the starting index for collision detection
    while(hashTable[index].data != EMPTY){
        if(hashTable[index].data == u){
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), RED, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 2, 3)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated

            hashTable[index].data = DELETED; // Mark the cell as deleted
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), RED, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 2, 3)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated
            break;
        }
        else{
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), YELLOW, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 1, 4)); // Add the current state of the hash table to the animation history
            hashTable[index].isUpdate = false; // Mark the cell as not updated
    
            hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), LIGHTGRAY, YELLOW, FULL_RADIUS, customFont, 20.0f, true);
            Animation.push_back(Snapshot(hashTable, deleteValueCode, 1, 4));
            hashTable[index].isUpdate = false; 
        }
        index = (index + 1) % capacity; 
        if(index == startIndex){
            break;
        }
    }
    hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), YELLOW, DARKGRAY, FULL_RADIUS, customFont, 20.0f, true);
    Animation.push_back(Snapshot(hashTable, deleteValueCode, 2, 3)); // Add the current state of the hash table to the animation history
    hashTable[index].isUpdate = false; // Mark the cell as not updated


    // Insert the new value
    for(int i = 0; i < capacity; ++i){
        hashTable[i] = NodeHashTable(hashTable[i].data, generatePosition(i), LIGHTGRAY, DARKGRAY, FULL_RADIUS, customFont, 20.0f, false);
    }

    if(size + 1 >= capacity){
        Animation.push_back(Snapshot(hashTable, insertValueCode, 0, 0));
        return;
    }
    else Animation.push_back(Snapshot(hashTable, insertValueCode, 0, 1));

    index = v % capacity; // Hash function to find the index
    startIndex = index; // Store the starting index for collision detection
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
    hashTable[index].data = v; // Insert the number into the hash table
    hashTable[index] = NodeHashTable(hashTable[index].data, generatePosition(index), SKYBLUE, DARKBLUE, FULL_RADIUS, customFont, 20.0f, true);
    Animation.push_back(Snapshot(hashTable, insertValueCode, 4, 4)); // Add the current state of the hash table to the animation history
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
    fclose(file);
}

void HashTable::handleEvents(){
    Vector2 mousePosition = GetMousePosition(); // Get the mouse position

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        int preInputType = inputType;
        // Check if the create button is clicked
        if (createButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 1;
            createNTextBox.createRandomValue(1, 99);
            createKTextBox.createRandomValue(1, createNTextBox.GetIntValue() / 2);
        } 
        
        // Check if the search button is clicked
        else if (searchButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 2;
            searchTextBox.createRandomValue(1, 999);
        }

        // Check if the insert button is clicked
        else if (addButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 3; 
            addTextBox.createRandomValue(1, 999);
        }

        else if (updateButton.IsHovered(mousePosition)){
            inputActive = true;
            inputType = 4;
            updateUTextBox.createRandomValue(1, 999);
            updateVTextBox.createRandomValue(1, 999);
        }

        // Check if the delete button is clicked
        else if (deleteButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 5;
            deleteTextBox.createRandomValue(1, 999);
        }

        //Check if the file button is clicked
        else if (fileButton.IsHovered(mousePosition)) {
            loadFile();
        }

        else if (clearButton.IsHovered(mousePosition)) {
            clear();
        }
    }

    if(inputActive){
        if(IsKeyPressed(KEY_ENTER)){
            if(inputType == 1){
                createNTextBox.Update();
                createKTextBox.Update();
                createRandomTable(createNTextBox.GetIntValue(), createKTextBox.GetIntValue());
            }
            else if(inputType == 2){
                searchTextBox.Update();
                search(searchTextBox.GetIntValue());
            }
            else if(inputType == 3){
                addTextBox.Update();
                insert(addTextBox.GetIntValue());
            }
            else if(inputType == 4){
                updateUTextBox.Update();
                updateVTextBox.Update();
                update(updateUTextBox.GetIntValue(), updateVTextBox.GetIntValue());
            }
            else if(inputType == 5){
                deleteTextBox.Update();
                remove(deleteTextBox.GetIntValue());
            }
            inputActive = false; // Deactivate input after processing
            inputType = 0; // Reset input type
        }
    }
}

void HashTable::drawButtons() {
    updatePseudocodeOn();
    addButton.Update();
    searchButton.Update();
    updateButton.Update();
    deleteButton.Update();
    createButton.Update();
    fileButton.Update();
    clearButton.Update();

    addButton.Draw();
    searchButton.Draw();
    updateButton.Draw();
    deleteButton.Draw();
    createButton.Draw();
    fileButton.Draw();
    clearButton.Draw();
    

    if(inputActive){
        if(inputType == 1){
            createNTextBox.Update();
            createKTextBox.Update();
            createNTextBox.Draw();
            createKTextBox.Draw();
        }
        else if(inputType == 2){
            searchTextBox.Update();
            searchTextBox.Draw();
        }
        else if(inputType == 3){
            addTextBox.Update();
            addTextBox.Draw();
        }
        else if(inputType == 4){
            updateUTextBox.Update();
            updateVTextBox.Update();
            updateUTextBox.Draw();
            updateVTextBox.Draw();
        }
        else if(inputType == 5){
            deleteTextBox.Update();
            deleteTextBox.Draw();
        }
    }

    DrawRectangleRec(togglePseudocodeButton, GREEN);
    if (isPseudocodeVisible) {
        // DrawRectangleLinesEx(togglePseudocodeButton, 2, BLACK);
        DrawTextEx(customFont, ">", { togglePseudocodeButton.x + (togglePseudocodeButton.width - MeasureText(">", 20)) / 2, togglePseudocodeButton.y + (togglePseudocodeButton.height - 20) / 2 }, 20, 2, WHITE);
    } else {
        // DrawRectangleLinesEx(togglePseudocodeButton, 2, BLACK);
        DrawTextEx(customFont, "<", { togglePseudocodeButton.x + (togglePseudocodeButton.width - MeasureText("<", 20)) / 2, togglePseudocodeButton.y + (togglePseudocodeButton.height - 20) / 2 }, 20, 2, WHITE);
    }
}

// Destructor
HashTable::~HashTable() {
    // No dynamic memory to free
}