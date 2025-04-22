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
    int offsetX = 15, offsetY = 120 + 600 - 7*buttonHeight - 6 * 5;
    createButton  = ButtonText(offsetX, offsetY, buttonWidth, buttonHeight, "Create");
    searchButton = ButtonText(offsetX, offsetY + buttonHeight + 5, buttonWidth, buttonHeight, "Search");
    addButton = ButtonText(offsetX, offsetY + 2*buttonHeight + 10, buttonWidth, buttonHeight, "Insert");    
    deleteButton = ButtonText(offsetX, offsetY + 3*buttonHeight + 15, buttonWidth, buttonHeight, "Delete");
    updateButton = ButtonText(offsetX, offsetY + 4*buttonHeight + 20, buttonWidth, buttonHeight, "Update");
    fileButton = ButtonText(offsetX, offsetY + 5*buttonHeight + 25, buttonWidth, buttonHeight, "File");
    clearButton = ButtonText(offsetX, offsetY + 6*buttonHeight + 30, buttonWidth, buttonHeight, "Clear");

    createTextBox = InputTextBox({createButton.bounds.x + createButton.bounds.width + 10, createButton.bounds.y, 120, createButton.bounds.height}, "N = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    searchTextBox = InputTextBox({searchButton.bounds.x + searchButton.bounds.width + 10, searchButton.bounds.y, 120, searchButton.bounds.height}, "V = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    addTextBox = InputTextBox({addButton.bounds.x + addButton.bounds.width + 10, addButton.bounds.y, 120, addButton.bounds.height}, "V = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    deleteTextBox = InputTextBox({deleteButton.bounds.x + deleteButton.bounds.width + 10, deleteButton.bounds.y, 120, deleteButton.bounds.height}, "V = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    updateUTextBox = InputTextBox({updateButton.bounds.x + updateButton.bounds.width + 10, updateButton.bounds.y, 120, updateButton.bounds.height}, "U = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
    updateVTextBox = InputTextBox({updateButton.bounds.x + updateButton.bounds.width + 10 + 125, updateButton.bounds.y, 120, updateButton.bounds.height}, "V = ", 3, 20, BLACK, BLACK, LIGHTGRAY);
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
    timeAnimation = 0.0f;
    curAnimation = 0;
    Animation.clear();


    inputActive = false;
    inputType = 0;

    
    resetTree(MAX_NODES);

    lineCount = 7;
    isPseudocodeVisible = false;
    togglePseudocodeButton = { (float)screenWidth-20, 520, 20, (float)lineCount * 30};
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
    float startY = 520;

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

Vector2 AVLTree::newPosVector2(Vector2 A, Vector2 B, float opacity){
    return {A.x + (B.x - A.x) * opacity, A.y + (B.y - A.y) * opacity};
}


void AVLTree::drawAnimation(SnapShot curShot, float opacity){
    if(isPseudocodeVisible){
        drawPseudocode(curShot.code, curShot.codeLine1, curShot.codeLine2); // Draw the pseudocode
    }
    if(!isLightMode){
        nodeColor = ((1) ? Color({200, 200, 200, 255}) : Color({220, 220, 220, 255}));
        nodeHighlightColor = ((1) ? Color({255, 191, 73, 255}) : Color({255, 184, 108, 255})); 
    }
    else{
        nodeColor = BLACK;
        nodeHighlightColor = ORANGE;
    }
    for(int i = 0; i < curShot.nodes.size(); ++i){
        NodeTree &u = curShot.nodes[i];
        if(!u.isUsed) continue;
        if(animationState == PAUSE) u.node.position = u.newPosition;
        else u.node.position = newPosVector2(u.oldPosition, u.newPosition, opacity);
        if(u.isDrawBalance) u.drawBalance();
        if(u.isDrawRemove) u.drawRemove();
        if(u.typeColor == 0) u.node.color = nodeColor;
        else if (u.typeColor == 1) u.node.color = nodeHighlightColor;
        u.node.draw(1.0f);
    }
    for (int i = 0; i < curShot.nodes.size(); ++i) {
        if (!curShot.nodes[i].isUsed) continue;
        NodeTree &u = curShot.nodes[i];
        if (u.left != -1) {
            NodeTree &v = curShot.nodes[u.left];
            if(u.typeColor == v.typeColor) DrawConnection(u.node.position, v.node.position, false, u.node.color, 4.0f, u.node.radius, v.node.radius);
            else DrawConnection(u.node.position, v.node.position, false, nodeColor, 4.0f, u.node.radius, v.node.radius);
        }
        if (u.right != -1) {
            NodeTree &v = curShot.nodes[u.right];
            if(u.typeColor == v.typeColor) DrawConnection(u.node.position, v.node.position, false, u.node.color, 4.0f, u.node.radius, v.node.radius);
            else DrawConnection(u.node.position, v.node.position, false, nodeColor, 4.0f, u.node.radius, v.node.radius);
        }
    }
}

void AVLTree::draw(){
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
        drawAnimation(Animation[curAnimation], opacity); // Draw the current animation frame
    }
    else{
        if(isPseudocodeVisible){
            drawPseudocode(nullptr, -1, -1); // Draw the pseudocode
        }
    }

    drawButtons();
}


//---------------------Handle Operations For AVL Tree---------------------//
void AVLTree::initializeAVLTree(int n){
    if(n == 0){
        std::cout << "Tree is empty\n";
        return;
    }
    Animation.clear();
    curAnimation = 0; // Reset the current animation index
    timeAnimation = 0.0f; // Reset the current time    
    animationState = PLAY;

    root = -1;
    for(int i = 0; i < capacity; ++i) nodes[i] = NodeTree();
    for(int i = 0; i < n; ++i){
        int value = GetRandomValue(1, 99); // Generate a random value between 1 and 99
        std::cout << value << ',';
        resetNodes();
        curAnimation = 0; // Reset the current animation index
        timeAnimation = 0.0f; // Reset the current time    
        animationState = PLAY;
        root = insertNode(root, value, 0, -1);
        generatePosition(root, root, 150, screenWidth - 30, 100, 100);
        Animation.push_back(SnapShot(nodes, root, insertValueCode, 6, 6));
    }
    std::cout << '\n';
}

//94,90,38,68,33,28,79,43,19,47,99,59,66,77,7,12,6,1,76,85,84,32,14,18,55,45,16
//67,17,64,90,66,6,4,53,61,22,94,69,83,87,56,36,88,13,32,83,6,30,79,84,64,91,74,91,2,5,9,94,92,48,2

void AVLTree::search(int value){
    Animation.clear();
    resetNodes();
    curAnimation = 0; // Reset the current animation index
    timeAnimation = 0.0f; // Reset the current time    
    animationState = PLAY;
    searchNode(root, value);
    resetNodes();
}

void AVLTree::insert(int value){
    Animation.clear();
    resetNodes();
    curAnimation = 0; // Reset the current animation index
    timeAnimation = 0.0f; // Reset the current time    
    animationState = PLAY;
    root = insertNode(root, value, 0, -1);
    generatePosition(root, root, 150, screenWidth - 30, 100, 100);
    Animation.push_back(SnapShot(nodes, root, insertValueCode, 6, 6));
    resetNodes();
}

void AVLTree::remove(int value){
    if(checkEmpty()){
        std::cout << "Tree is empty\n";
        return;
    }
    Animation.clear();
    resetNodes();
    curAnimation = 0; // Reset the current animation index
    timeAnimation = 0.0f; // Reset the current time    
    animationState = PLAY;
    root = deleteNode(root, value, 0, -1);
    generatePosition(root, root, 150, screenWidth - 30, 100, 100);
    Animation.push_back(SnapShot(nodes, root, deleteValueCode, 6, 6)); 
    resetNodes();
}

void AVLTree::update(int u, int v){
    Animation.clear();
    resetNodes();
    curAnimation = 0; // Reset the current animation index
    timeAnimation = 0.0f; // Reset the current time    
    animationState = PLAY;
    if(!checkEmpty()){
        root = deleteNode(root, u, 0, -1);
        generatePosition(root, root, 150, screenWidth - 30, 100, 100);
        Animation.push_back(SnapShot(nodes, root, deleteValueCode, 6, 6)); 
        resetNodes();
    }
    else std::cout << "Tree is empty!!!\n";

    root = insertNode(root, v, 0, -1);
    generatePosition(root, root, 150, screenWidth - 30, 100, 100);
    Animation.push_back(SnapShot(nodes, root, insertValueCode, 6, 6));
    resetNodes();
}

void AVLTree::loadFile(){
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
    fscanf(file, "%d", &n); // Read the number of elements
    if(n == 0) return;
    for(int i = 0; i < n; ++i){
        int value;
        fscanf(file, "%d", &value); // Read the elements from the file
        root = insertNode(root, value, 0, -1);
        generatePosition(root, root, 150, screenWidth - 30, 100, 100);
        Animation.push_back(SnapShot(nodes, root, insertValueCode, 6, 6));
        resetNodes();
    }
    fclose(file);
}

void AVLTree::clear(){
    Animation.clear();
    root = -1;
    for(int i = 0; i < nodes.size(); ++i) nodes[i] = NodeTree();
    curAnimation = 0; // Reset the current animation index
    timeAnimation = 0.0f; // Reset the current time    
    animationState = PLAY;
}

void AVLTree::handleEvents(){
    Vector2 mousePosition = GetMousePosition(); // Get the mouse position

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        // Check if the create button is clicked
        if (createButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 1;
            createTextBox.createRandomValue(1, 99);
        }
        else if (searchButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 2; // Set input type to search
            searchTextBox.createRandomValue(1, 999);
        }

        // Check if the insert button is clicked
        else if (addButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 3; // Set input type to insert
            addTextBox.createRandomValue(1, 999);
        }

        // Check if the delete button is clicked
        else if (deleteButton.IsHovered(mousePosition)) {
            inputActive = true;
            inputType = 4; // Set input type to delete
            deleteTextBox.createRandomValue(1, 999);  
        }

        else if (updateButton.IsHovered(mousePosition)){
            inputActive = true;
            inputType = 5;
            updateUTextBox.createRandomValue(1, 999);
            updateVTextBox.createRandomValue(1, 999);
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
                initializeAVLTree(createTextBox.GetIntValue());
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
                deleteTextBox.Update();
                remove(deleteTextBox.GetIntValue());
            }
            else if(inputType == 5){
                updateUTextBox.Update();
                updateVTextBox.Update();
                update(updateUTextBox.GetIntValue(), updateVTextBox.GetIntValue());
            }
            inputActive = false; // Deactivate input after processing
            inputType = 0; // Reset input type
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
            searchTextBox.Draw();
        }
        else if(inputType == 3){
            addTextBox.Update();
            addTextBox.Draw();
        }
        else if(inputType == 4){
            deleteTextBox.Update();
            deleteTextBox.Draw();
        }
        else if(inputType == 5){
            updateUTextBox.Update();                      
            updateVTextBox.Update();
            updateUTextBox.Draw();
            updateVTextBox.Draw();
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

AVLTree::~AVLTree() {
    // Destructor implementation
    // Clean up resources if needed
}