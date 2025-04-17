#ifndef AVLTREE_H
#define AVLTREE_H

#include <raylib.h>
#include <string>
#include <vector>
#include <algorithm>
#include "var.h"
#include "manager.h"
#include "tinyfiledialogs.h"
#include "node.h"

const int MAX_NODES = 256; // Số lượng nút tối đa trong cây AVL
class AVLTree {
public:


    struct NodeTree{
        Node node;
        int value;
        int height;
        int rank;
        int left;
        int right;
        int parent;
        int bf; // Balance factor
        bool isUsed;
        bool isUpdate;
        bool isHightlight;
        bool isDrawBalance;
        bool isDrawRemove;
        Vector2 oldPosition; // Vị trí cũ của nút (để vẽ animation)
        Vector2 newPosition; // Vị trí mới của nút (để vẽ animation)

        NodeTree(){
            value = 0;
            height = 0;
            left = -1;
            right = -1;
            parent = -1;
            bf = 0;
            isUsed = false;
            isUpdate = false;
            isHightlight = false;
            isDrawBalance = false;
            isDrawRemove = false;
            oldPosition = {(float)screenWidth/2.0f, (float)screenHeight - 120.0f};
            newPosition = {(float)screenWidth/2.0f, (float)screenHeight - 120.0f};
            node = Node("", {(float)screenWidth/2.0f, (float)screenHeight - 120.0f}, BLACK, radius - 5.0f, customFont, fontSize);
        };
        NodeTree(int val, int h, int l, int r, int p, bool used) : value(val), height(h), left(l), right(r), parent(p), isUsed(used) {
            node = Node(std::to_string(value), {(float)screenWidth/2.0f, (float)screenHeight - 120.0f}, BLACK, radius - 5.0f, customFont, fontSize);
            bf = 0;
            isUpdate = false;
            isHightlight = false;
            isDrawBalance = false;
            isDrawRemove = false;
        }

        void drawBalance() {
            // if (isDrawBalance) { // Only draw if the flag is set
                std::string balanceText = "bf=" + std::to_string(bf);
                Vector2 textPosition = {node.position.x - MeasureText(balanceText.c_str(), 20) / 2.0f, node.position.y + 30}; // Position above the node
                DrawTextEx(customFont, balanceText.c_str(), {textPosition.x, textPosition.y}, 20, 1, RED);
                // DrawText(balanceText.c_str(), textPosition.x, textPosition.y, 20, RED); // Draw the balance factor in red
            // }
        }

        void drawRemove() {
            // if (isDrawRemove) { // Only draw if the flag is set
                std::string removeText = "Remove";
                Vector2 textPosition = {node.position.x - MeasureText(removeText.c_str(), 20) / 2.0f, node.position.y + 30}; // Position above the node
                DrawTextEx(customFont, removeText.c_str(), {textPosition.x, textPosition.y}, 20, 1, RED);
            // }
        }
    };

    struct SnapShot{
        std::vector<NodeTree> nodes;
        int root;                   // Chỉ số của nút gốc
        int checkIndex;             // Chỉ số nút đang kiểm tra (nếu có)
        float insertTimer;          // Thời gian animation hiện tại
        int insertingNumber;        // Số đang được chèn (nếu có)
        const char** code = nullptr; // Mã lệnh tương ứng với trạng thái hiện tại
        int codeLine1 = -1, codeLine2 = -1;

        SnapShot(){};
        SnapShot(std::vector<NodeTree> newNodes, int newRoot, const char** newCode = nullptr, int newCodeLine1 = -1, int newCodeLine2 = -1) {
            nodes = newNodes;
            root = newRoot;
            code = newCode;
            codeLine1 = newCodeLine1; // Dòng đầu tiên của mã lệnh
            codeLine2 = newCodeLine2; // Dòng thứ hai của mã lệnh
        }
        SnapShot(std::vector<NodeTree> newNodes, int newRoot, int newCheckIndex, float newInsertTimer, int newInsertingNumber, const char** newCode = nullptr, int newCodeLine1 = -1, int newCodeLine2 = -1) {
            nodes = newNodes;
            root = newRoot;
            checkIndex = newCheckIndex;
            insertTimer = newInsertTimer;
            insertingNumber = newInsertingNumber;
            code = newCode; // Mã lệnh tương ứng với trạng thái hiện tại
            codeLine1 = newCodeLine1; // Dòng đầu tiên của mã lệnh
            codeLine2 = newCodeLine2; // Dòng thứ hai của mã lệnh
        }
    };

    std::vector<NodeTree> nodes; // Cây AVL (mỗi nút chứa giá trị và chiều cao)
    std::vector<NodeTree> nodesAnimation;
    int capacity;
    int root;

    //--------------------------AVL Tree--------------------------//
    void resetTree(int newCapacity){
        capacity = newCapacity;
        root = -1;
        nodes.resize(capacity);
    }

    int newID(){
        for(int i = 0; i < capacity; ++i){
            if(!nodes[i].isUsed) return i;
        }
        return -1;
    }

    void deleteID(int idx){
        nodes[idx] = NodeTree(0, 0, -1, -1, -1, false); // Đánh dấu nút là không sử dụng
    }

    void initNode(int idx, int value){
        nodes[idx] = NodeTree(value, 1, -1, -1, -1, 1); // Khởi tạo nút với giá trị và chiều cao 1
    }
    
    bool checkEmpty(){
        for(int i = 0; i < capacity; ++i){
            if(nodes[i].isUsed) return false;
        }
        return true;
    }

    int getHeight(int idx) {
        if (idx == -1) return 0;
        return nodes[idx].height;
    }

    int getBalance(int idx){
        if(idx == -1) return 0;
        return getHeight(nodes[idx].left) - getHeight(nodes[idx].right);
    }

    void updateHeight(int idx){
        if(idx == -1) return;
        nodes[idx].height = std::max(getHeight(nodes[idx].left), getHeight(nodes[idx].right)) + 1;        
    }
    

//-----------------------------------------------------------------------------------------

    // Rotate the subtree rooted at y to the right
    int rotateRight(int y, const char** code = nullptr, int codeLine1 = -1, int codeLine2 = -1) {
        generatePosition(root, root, 150, screenWidth - 30, 100, 100);
        int x = nodes[y].left;
        int T2 = nodes[x].right;
        
        // Highlight các nút liên quan
        nodes[y].node.color = nodeHighlightColor; nodes[y].isDrawBalance = true;
        nodes[x].node.color = nodeHighlightColor; nodes[x].isDrawBalance = true;
        if (T2 != -1){
            nodes[T2].node.color = nodeHighlightColor;
            nodes[T2].isDrawBalance = true;
        }
        Animation.push_back(SnapShot(nodes, root, code, codeLine1, codeLine2));
    
        nodes[x].right = y;
        nodes[y].left = T2;

        if(nodes[y].parent != -1){
            if(nodes[nodes[y].parent].left == y) nodes[nodes[y].parent].left = x;
            else nodes[nodes[y].parent].right = x;
        }

        nodes[x].parent = nodes[y].parent;
        nodes[y].parent = x;
        
        if(T2 != -1) nodes[T2].parent = y;
        
        if(y == root) root = x;
        generatePosition(root, root, 150, screenWidth - 30, 100, 100);
        Animation.push_back(SnapShot(nodes, root, code, codeLine1, codeLine2));

        updateHeight(y);
        updateHeight(x);


        nodes[y].node.color = nodeColor; nodes[y].isDrawBalance = false;
        nodes[x].node.color = nodeColor; nodes[x].isDrawBalance = false;
        if (T2 != -1){
            nodes[T2].node.color = nodeColor;
            nodes[T2].isDrawBalance = false;
        }
        // generatePosition(root, root, 150, screenWidth - 30, 100, 100);

        return x;
    }

    int rotateLeft(int x, const char** code = nullptr, int codeLine1 = -1, int codeLine2 = -1){
        generatePosition(root, root, 150, screenWidth - 30, 100, 100);
        int y = nodes[x].right;
        int T2 = nodes[y].left;

        nodes[x].node.color = nodeHighlightColor; nodes[x].isDrawBalance = true;
        nodes[y].node.color = nodeHighlightColor; nodes[y].isDrawBalance = true;
        if (T2 != -1){
            nodes[T2].node.color = nodeHighlightColor;
            nodes[T2].isDrawBalance = true;
        }
        Animation.push_back(SnapShot(nodes, root, code, codeLine1, codeLine2));

        nodes[y].left = x;
        nodes[x].right = T2;

        if(nodes[x].parent != -1){
            if(nodes[nodes[x].parent].left == x) nodes[nodes[x].parent].left = y;
            else nodes[nodes[x].parent].right = y;
        }

        nodes[y].parent = nodes[x].parent;
        nodes[x].parent = y;
        if(T2 != -1) nodes[T2].parent = x;
        
        if(x == root) root = y;
        generatePosition(root, root, 150, screenWidth - 30, 100, 100);
        Animation.push_back(SnapShot(nodes, root, code, codeLine1, codeLine2));
        
        updateHeight(x);
        updateHeight(y);
        

        nodes[x].node.color = nodeColor; nodes[x].isDrawBalance = false;
        nodes[y].node.color = nodeColor; nodes[y].isDrawBalance = false;
        if (T2 != -1){
            nodes[T2].node.color = nodeColor;
            nodes[T2].isDrawBalance = false;
        }
        // generatePosition(root, root, 150, screenWidth - 30, 100, 100);

        return y;
    }

    int insertNode(int idx, int value, int pos, int parent){
        if(idx == -1){
            idx = newID();
            if(idx == -1){
                std::cout << "Het cho mat roi hic :((\n";
                return -1; // Không còn chỗ trống trong cây
            }
            nodes[idx] = NodeTree(value, 1, -1, -1, -1, 1);
            nodes[idx].oldPosition = {(float)screenWidth/2.0f, (float)screenHeight - 120.0f};
            nodes[idx].newPosition = {(float)screenWidth/2.0f, (float)screenHeight - 120.0f};
            nodes[idx].parent = parent;
            nodes[idx].node.color = nodeHighlightColor;
            nodes[idx].isUpdate = true;
            if(parent != -1 && parent != idx){
                if(pos == 0) nodes[parent].left = idx;
                else nodes[parent].right = idx;
            }
            generatePosition(root, root, 150, screenWidth - 30, 100, 100);
            if(parent != -1 && parent != idx) {
                nodes[idx].oldPosition = {nodes[parent].newPosition.x + 25, nodes[parent].newPosition.y + 25};
            }
            // std::cout << nodes[idx].oldPosition.x << ' ' << nodes[idx].oldPosition.y << ' ' << nodes[idx].newPosition.x << ' ' << nodes[idx].newPosition.y << '\n';
            Animation.push_back(SnapShot(nodes, root, insertValueCode, 0, 0)); 
            
            for(int i = 0; i < nodes.size(); ++i){
                nodes[i].node.color = nodeColor;
            }

            nodes[idx].node.color = nodeHighlightColor;
            nodes[idx].oldPosition = nodes[idx].newPosition;
            nodes[idx].isUpdate = true;
            Animation.push_back(SnapShot(nodes, root, insertValueCode, 0, 0));
            nodes[idx].node.color = nodeColor;
            nodes[idx].isUpdate = false;

            return idx;
        }

        nodes[idx].node.color = nodeHighlightColor;
        nodes[idx].isUpdate = true;
        Animation.push_back(SnapShot(nodes, root, insertValueCode, 0, 0));
        nodes[idx].isUpdate = false;


        if(value < nodes[idx].value){
            nodes[idx].left = insertNode(nodes[idx].left, value, 0, idx);
        }
        else if (value > nodes[idx].value){
            nodes[idx].right = insertNode(nodes[idx].right, value, 1, idx);
        }
        else return idx;

        updateHeight(idx);
        int balance = getBalance(idx);

        generatePosition(root, root, 150, screenWidth - 30, 100, 100);
        nodes[idx].node.color = nodeHighlightColor;
        nodes[idx].isUpdate = true;
        nodes[idx].bf = balance;
        nodes[idx].isDrawBalance = true;
        Animation.push_back(SnapShot(nodes, root, insertValueCode, 0, 0));
        nodes[idx].node.color = nodeColor;
        nodes[idx].isUpdate = false;
        nodes[idx].isDrawBalance = false;

        if (balance > 1 && value < nodes[nodes[idx].left].value){ // LL case
            return rotateRight(idx, insertValueCode, 1, 2);
        }
        if (balance < -1 && value > nodes[nodes[idx].right].value){ // RR case
            return rotateLeft(idx, insertValueCode, 1, 4);
        }
        if (balance > 1 && value > nodes[nodes[idx].left].value){ // LR case
            nodes[idx].left = rotateLeft(nodes[idx].left, insertValueCode, 1, 3);
            return rotateRight(idx, insertValueCode, 1, 3);
        }
        if (balance < -1 && value < nodes[nodes[idx].right].value){ // RL case
            nodes[idx].right = rotateRight(nodes[idx].right, insertValueCode, 1, 5);
            return rotateLeft(idx, insertValueCode, 1, 5);
        }

        return idx;
    }

    int findMin(int idx) {
        if (idx == -1) return -1;
    
        // Highlight nút hiện tại đang được kiểm tra
        nodes[idx].node.color = nodeHighlightColor;
        nodes[idx].isUpdate = true;
        Animation.push_back(SnapShot(nodes, root, deleteValueCode, 0, 0)); // Lưu trạng thái khi kiểm tra nút
        nodes[idx].isUpdate = false;
    
        while (nodes[idx].left != -1) {
            idx = nodes[idx].left;
    
            // Highlight nút tiếp theo
            nodes[idx].node.color = nodeHighlightColor;
            nodes[idx].isUpdate = true;
            Animation.push_back(SnapShot(nodes, root, deleteValueCode, 0, 0)); // Lưu trạng thái khi di chuyển đến nút tiếp theo
            nodes[idx].isUpdate = false;
        }
    
        return idx;
    }

    int deleteNode(int idx, int value, int pos, int parent) {
        if (idx == -1) return -1;
        
        nodes[idx].parent = parent;
        nodes[idx].node.color = nodeHighlightColor;
        nodes[idx].isUpdate = true;
        Animation.push_back(SnapShot(nodes, root, deleteValueCode, 0, 0));
        nodes[idx].isUpdate = false;

        if (value < nodes[idx].value) {
            nodes[idx].left = deleteNode(nodes[idx].left, value, 0, idx);
        } else if (value > nodes[idx].value) {
            nodes[idx].right = deleteNode(nodes[idx].right, value, 1, idx);
        } else {
            // Highlight nút cần xóa
            nodes[idx].node.color = nodeHighlightColor;
            nodes[idx].isDrawRemove = true;
            Animation.push_back(SnapShot(nodes, root, deleteValueCode, 0, 0));
    
            // Trường hợp nút có tối đa 1 con
            if (nodes[idx].left == -1 || nodes[idx].right == -1) {
                int temp = (nodes[idx].left != -1) ? nodes[idx].left : nodes[idx].right;
    
                if (temp == -1) { // Không có con
                    deleteID(idx);
                    if(parent != -1 && pos == 0) nodes[parent].left = -1;
                    else if(parent != -1 && pos == 1) nodes[parent].right = -1;
                    idx = -1;

                    Animation.push_back(SnapShot(nodes, root, deleteValueCode, 0, 0));
                    for(int i = 0; i < nodes.size(); ++i) nodes[i].node.color = nodeColor;

                    return -1;
                } else { // Có 1 con
                    // nodes[temp].parent = nodes[idx].parent;
                    // idx = temp;

                    if(parent != -1 && pos == 0) nodes[parent].left = temp;
                    else if(parent != -1 && pos == 1) nodes[parent].right = temp;
                    nodes[temp].parent = parent;
                    nodes[temp].node.color = nodeHighlightColor;
                    nodes[temp].isUpdate = true;
                    nodes[temp].newPosition = nodes[idx].newPosition;
                    deleteID(idx);
                    Animation.push_back(SnapShot(nodes, root, deleteValueCode, 0, 0));

                    for(int i = 0; i < nodes.size(); ++i) nodes[i].node.color = nodeColor;
                    nodes[temp].isUpdate = false;
                    nodes[temp].oldPosition = nodes[temp].newPosition;
                    nodes[temp].isDrawRemove = false;
                    idx = temp;
                }
            } else {
                // Trường hợp nút có 2 con
                int temp = findMin(nodes[idx].right);
                if(temp == nodes[idx].right){
                    nodes[idx].value = nodes[temp].value;
                    nodes[idx].node.data = std::to_string(nodes[idx].value);
                    nodes[idx].newPosition = nodes[temp].newPosition;
                    nodes[idx].oldPosition = nodes[temp].newPosition;

                    int tempRight = nodes[temp].right;
                    nodes[idx].right = tempRight;

                    if(tempRight != -1) nodes[tempRight].parent = idx;

                    deleteID(temp);
                    generatePosition(root, root, 150, screenWidth - 30, 100, 100);
                    Animation.push_back(SnapShot(nodes, root, deleteValueCode, 0, 0));

                    nodes[idx].isDrawRemove = false;
                    for(int i = 0; i < nodes.size(); ++i){
                        nodes[i].node.color = nodeColor;
                        nodes[i].oldPosition = nodes[i].newPosition;
                    }
                    return idx;
                }
                
                // if(parent != -1 && pos == 0) nodes[parent].left = idx;
                // else if(parent != -1 && pos == 1) nodes[parent].right = idx;
                
                nodes[idx].value = nodes[temp].value;
                nodes[idx].oldPosition = nodes[temp].newPosition;
                nodes[idx].node.data = std::to_string(nodes[idx].value);
                if(nodes[idx].left != -1) nodes[nodes[idx].left].node.color = nodeHighlightColor;
                if(nodes[idx].right != -1) nodes[nodes[idx].right].node.color = nodeHighlightColor;
                nodes[nodes[temp].parent].left = nodes[temp].right;
                int tempIDRight = nodes[temp].right;
                nodes[tempIDRight].parent = nodes[temp].parent;

                if(nodes[temp].right != -1) nodes[nodes[temp].right].newPosition = nodes[temp].newPosition;
                deleteID(temp);
    
                // Highlight nút thay thế 
                nodes[idx].node.color = nodeHighlightColor;
                Animation.push_back(SnapShot(nodes, root, deleteValueCode, 0, 0));    

                for(int i = 0; i < capacity; ++i) nodes[i].node.color = nodeColor;

                nodes[idx].oldPosition = nodes[idx].newPosition;           
                nodes[idx].isDrawRemove = false;
                if(nodes[idx].left != -1) nodes[nodes[idx].left].node.color = nodeColor;
                if(nodes[idx].right != -1) nodes[nodes[idx].right].node.color = nodeColor; 
                if(tempIDRight != -1) nodes[tempIDRight].oldPosition = nodes[tempIDRight].newPosition;
                // if(nodes[nodes[temp].parent].left != -1) nodes[nodes[nodes[temp].parent].left].oldPosition = nodes[nodes[temp].parent].newPosition;

                nodes[idx].right = deleteNode(nodes[idx].right, nodes[idx].value, 1, idx);
            }
        }
    
        if (idx == -1) return -1;
    
        // Cập nhật chiều cao và lưu trạng thái
        updateHeight(idx);
        int balance = getBalance(idx);
        
        generatePosition(root, root, 150, screenWidth - 30, 100, 100);
        nodes[idx].bf = balance;
        nodes[idx].isDrawBalance = true;
        nodes[idx].node.color = nodeHighlightColor;
        Animation.push_back(SnapShot(nodes, root, deleteValueCode, 0, 0));
        nodes[idx].isDrawBalance = false;
        nodes[idx].node.color = nodeColor;
    
        // Kiểm tra và thực hiện cân bằng
        if (balance > 1 && getBalance(nodes[idx].left) >= 0) { // LL case
            return rotateRight(idx, deleteValueCode, 1, 2);
        }
        if (balance > 1 && getBalance(nodes[idx].left) < 0) { // LR case
            nodes[idx].left = rotateLeft(nodes[idx].left, deleteValueCode, 1, 3);
            return rotateRight(idx, deleteValueCode, 1, 3);
        }
        if (balance < -1 && getBalance(nodes[idx].right) <= 0) { // RR case
            return rotateLeft(idx, deleteValueCode, 1, 4);
        }
        if (balance < -1 && getBalance(nodes[idx].right) > 0) { // RL case
            nodes[idx].right = rotateRight(nodes[idx].right, deleteValueCode, 1, 5);
            return rotateLeft(idx, deleteValueCode, 1, 5);
        }
    
        return idx;
    }

    int searchNode(int idx, int value) {
        if (idx == -1){
            for(int i = 0; i < capacity; ++i) nodes[i].node.color = nodeColor;
            Animation.push_back(SnapShot(nodes, root, searchValueCode, 0, 0));
            return -1;
        }
        
        if (nodes[idx].value == value){
            for(int i = 0; i < capacity; ++i) nodes[i].node.color = nodeColor;
            nodes[idx].node.color = nodeHighlightColor;
            nodes[idx].isUpdate = true;
            Animation.push_back(SnapShot(nodes, root, searchValueCode, 1, 1));
            nodes[idx].isUpdate = false;            
            return idx;
        }

        if (value < nodes[idx].value) {
            nodes[idx].node.color = nodeHighlightColor;
            nodes[idx].isUpdate = true;
            Animation.push_back(SnapShot(nodes, root, searchValueCode, 2, 2));
            nodes[idx].isUpdate = false;
            return searchNode(nodes[idx].left, value);
        }

        nodes[idx].node.color = nodeHighlightColor;
        nodes[idx].isUpdate = true;
        Animation.push_back(SnapShot(nodes, root, searchValueCode, 3, 3));
        nodes[idx].isUpdate = false;
        return searchNode(nodes[idx].right, value);
    }

    void generatePosition(int idx, int par, float l, float r, float h, float delh){
        if (idx == -1) return;
        
        NodeTree &cur = nodes[idx];
        cur.oldPosition = cur.newPosition;
        cur.newPosition = {(l + r) / 2.0f, h};
        
        if(cur.left != -1){
            generatePosition(cur.left, idx, l, (l + r) / 2.0f, h + delh, delh);
        }
        if(cur.right != -1){
            generatePosition(cur.right, idx, (l + r) / 2.0f, r, h + delh, delh);
        }
    }

    void resetNodes(){
        for(int i = 0; i < nodes.size(); ++i){
            nodes[i].node.color = nodeColor;
            nodes[i].oldPosition = nodes[i].newPosition; 
            nodes[i].isUpdate = false;
            nodes[i].isDrawBalance = false;
        }
    }

    // Create a compare Color in raylib
    bool compareColor(Color a, Color b){
        return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
    }

    // Pseudocode for hash function
    const char* insertValueCode[7] = {
        "Insert v\n",
        "Check balance factor (bf):\n",
        "  LL: trt_Right\n",
        "  LR: left.rt_Left, rt_Right\n",
        "  RR: rt_Left\n",
        "  RL: right.rt_Right, rt_Left\n",
        "  this is balanced"
    };

    const char* searchValueCode[7] = {
        "if (this == NULL) return NOT_FOUND\n",
        "if (this.data == value) return FOUND\n",
        "if (value < this.data) searchLeft\n",
        "if (value > this.data) searchRight\n",
        "",
        "",
        "",
    };

    const char* deleteValueCode[7] = {
        "Delete v\n",
        "Check balance factor (bf):\n",
        "  LL: rt_Right\n",
        "  LR: left.rt_Left, rt_Right\n",
        "  RR: rt_Left\n",
        "  RL: right.rt_Right, rt_Left\n",
        "  this is balanced"
    };

    enum ANIMATION_STATE{PAUSE, PLAY, REPLAY};
    ANIMATION_STATE animationState; // Trạng thái animation hiện tại

    int lineCount;
    Rectangle togglePseudocodeButton;
    bool isPseudocodeVisible;
    void drawPseudocode(const char**, int, int);
    void updatePseudocodeOn();

    bool inputActive;
    int inputType;

    std::vector<SnapShot> Animation;
    float timeAnimation = 0.0f; // Thời gian hiện tại của animation
    int curAnimation = 0;   // Chỉ số animation hiện tại

    std::vector<button> remote;

    Color edgeColor = BLACK;
    Color edgeHighlightColor = ORANGE;
    Color nodeColor = BLACK;
    Color nodeHighlightColor = ORANGE;

    ButtonText createButton;
    ButtonText searchButton;
    ButtonText addButton;
    ButtonText deleteButton;
    ButtonText updateButton;
    ButtonText fileButton; 
    ButtonText clearButton;

    InputTextBox createTextBox;
    InputTextBox searchTextBox;
    InputTextBox addTextBox;
    InputTextBox deleteTextBox;
    InputTextBox updateUTextBox, updateVTextBox;

    AVLTree();
    Vector2 newPosVector2(Vector2, Vector2, float);
    void init();
    void initializeAVLTree(int);
    void search(int);
    void insert(int);
    void remove(int);
    void update(int, int);
    void loadFile();
    void clear();
    void handleRemote();
    void handleEvents();
    void drawAnimation(SnapShot, float);
    void draw();
    void drawButtons();
    ~AVLTree();
};



#endif // AVLTREE_H