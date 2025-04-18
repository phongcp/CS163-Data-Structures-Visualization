#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include "var.h"
#include "node.h"
#include "tinyfiledialogs.h"


class Trie{
public:
    const int MAX_NODES = 256;

    struct TrieNode{
        Node node;
        char ch;
        int cnt;
        int exist;
        int child[26];
        int typeColor = 0; // 0: normal, 1: highlight, 2: delete, 3: exist
        Vector2 oldPosition;
        Vector2 newPosition;

        TrieNode(){
            ch = ' ';
            cnt = exist = 0;
            typeColor = 0;
            memset(child, -1, sizeof(child));
            node = Node("", {-300, -300}, BLACK, radius - 5.0f, customFont, fontSize);
        };
    };

    struct SnapShot{
        std::vector<TrieNode>nodes;
        const char** code= nullptr;
        int codeLine1;
        int codeLine2;
        int codeLine3;
        bool isInsertState;

        SnapShot(){};
        SnapShot(std::vector<TrieNode> newNodes, const char** newCode, int newCodeLine1, int newCodeLine2, int newCodeLine3, bool insertState = false) {
            nodes = newNodes;
            code = newCode;
            codeLine1 = newCodeLine1;
            codeLine2 = newCodeLine2;
            codeLine3 = newCodeLine3;
            isInsertState = insertState;
        }
    };

    std::vector<TrieNode> nodes;
    int capacity;
    int cur;
    int root;

    void resetTrie(int num){
        nodes.assign(num, TrieNode());
        root = 0;
        cur = 0;
        nodes[0].node = Node("", {(float)screenWidth/2.0f, 120}, BLACK, radius, customFont, fontSize);
        memset(nodes[0].child, -1, sizeof(nodes[0].child));
        nodes[0].exist = nodes[0].cnt = 0;
    }

    int new_node(){
        ++cur;
        memset(nodes[cur].child, -1, sizeof(nodes[cur].child));
        nodes[cur].exist = nodes[cur].cnt = 0;
        return cur;
    }

    void addString(std::string word){
        int pos = 0;
        nodes[pos].typeColor = 1;
        Animation.push_back(SnapShot(nodes, insertCode, 0, 0, 0, true));
        for(char f : word){
            int c = f - 'A';
            if(nodes[pos].child[c] == -1){
                nodes[pos].child[c] = new_node();
                int v = nodes[pos].child[c];
                nodes[v].node.data = std::string(1, f);
                
                adjustPosition();
                nodes[v].typeColor = 1;
                nodes[v].oldPosition = {nodes[pos].newPosition.x, nodes[pos].newPosition.y + nodes[pos].node.radius + 5};
                Animation.push_back(SnapShot(nodes, insertCode, 1, 2, 3, true));
                // nodes[v].oldPosition = nodes[v].newPosition;
                // adjustPosition();
                // std::cout << nodes[v].node.data << ' ' << nodes[v].node.color.r << ' ' << nodes[v].node.color.g << ' ' << nodes[v].node.color.b << '\n';
            }
            else{
                nodes[nodes[pos].child[c]].typeColor = 1;
                Animation.push_back(SnapShot(nodes, insertCode, 1, 4, -1, true));
            }
            pos = nodes[pos].child[c];
            ++nodes[pos].cnt;
        }
        ++nodes[pos].exist;
        for(int i = 0; i < nodes.size(); ++i){
            if(nodes[i].exist) nodes[i].typeColor = 3;
            else nodes[i].typeColor = 0;
            nodes[i].oldPosition = nodes[i].newPosition;
        }
        Animation.push_back(SnapShot(nodes, insertCode, 5, -1, -1, true));
    }

    bool deleteStringRecursive(int pos, std::string &word, int i){
        nodes[pos].typeColor = 1;
        Animation.push_back(SnapShot(nodes, deleteCode, 1, -1, -1, false));
        if(i != (int)(word.size())){
            int c = word[i] - 'A';
            bool isChildDeleted = deleteStringRecursive(nodes[pos].child[c], word, i + 1);
            if(isChildDeleted){
                nodes[pos].child[c] = -1;
                adjustPosition();
                Animation.push_back(SnapShot(nodes, deleteCode, 2, 5, 6, false));   
                // for(int i = 0; i < nodes.size(); ++i) nodes[i].oldPosition = nodes[i].newPosition; 
            }
        }
        else --nodes[pos].exist;


        if(pos != 0){
            nodes[pos].typeColor = 2;
            Animation.push_back(SnapShot(nodes, deleteCode, 2, 3, 4, false));
            --nodes[pos].cnt;
            nodes[pos].typeColor = 0;
            if(nodes[pos].cnt == 0) return true;
        }
        return false;
    }

    void deleteString(std::string word){
        if(findString(word) == false){
            Animation.push_back(SnapShot(nodes, deleteCode, 0, -1, -1, false));
            return;
        }
        Animation.push_back(SnapShot(nodes, deleteCode, 1, -1, -1, false));
        deleteStringRecursive(0, word, 0);
        adjustPosition();
        for(int i = 0; i < nodes.size(); ++i){
            if(nodes[i].exist > 0) nodes[i].typeColor = 3;
            else nodes[i].typeColor = 0;
        }
        Animation.push_back(SnapShot(nodes, deleteCode, 7, -1, -1));
    }

    bool findString(std::string word){
        int pos = 0;
        for(char f : word){
            int c = f - 'A';
            if(nodes[pos].child[c] == -1) return false;
            pos = nodes[pos].child[c];
        }
        return (nodes[pos].exist > 0);     
    }

    void searchString(std::string word){
        int pos = 0;
        nodes[pos].typeColor = 1;
        Animation.push_back(SnapShot(nodes, searchCode, 0, 0, 0));
        for(char f : word){
            int c = f - 'A';
            if(nodes[pos].child[c] == -1){
                // std::cout << "BAO";
                for(int i = 0; i < nodes.size(); ++i) if (nodes[i].typeColor != 3) nodes[i].typeColor = 0;
                Animation.push_back(SnapShot(nodes, searchCode, 1, 2, 3));
                return;
            }
            else{
                nodes[nodes[pos].child[c]].typeColor = 1;
                Animation.push_back(SnapShot(nodes, searchCode, 1, 4, -1));
            }
            pos = nodes[pos].child[c];
        }
        if(nodes[pos].exist > 0){
            Animation.push_back(SnapShot(nodes, searchCode, 5, -1, -1));
            return;
        }
        for(int i = 0; i < nodes.size(); ++i) if (nodes[i].typeColor != 3) nodes[i].typeColor = 0;
        Animation.push_back(SnapShot(nodes, searchCode, 6, -1, -1));
        return;        
    }

    float generatePosition(int u, float t, float f, float h, float delh, float r, int isSavePos);
    void adjustPosition();
    bool compareColor(Color a, Color b){
        return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
    }

    // Pseudocode for hash function
    const char* insertCode[8] = {
        "node = root\n",
        "for(char ch : word)\n",
        "  if node.child[ch] == NULL\n",
        "    node.child[ch] = new TrieNode(ch)\n",
        "  node = node.child[ch]\n",
        "node.isEndOfWord = true\n",
        "",
        ""
    };

    const char* searchCode[8] = {
        "node = root\n",
        "for(char ch : word)\n",
        "  if node.child[ch] == NULL\n",
        "    return NOT_FOUND\n",
        "  cur = cur.child[ch]\n",
        "if cur.isEndOfWord return FOUND\n",
        "return NOT_FOUND\n",
        ""
    };

    const char* deleteCode[8] = {
        "if (search(word) == NOT_FOUND) return NOT_FOUND\n",
        "node = endOfWordNode(word)\n",
        "for(char ch : rev(word))\n",
        "  node.numChild--\n",
        "  node = node.parent\n",
        "  if node.child[ch].numChild == 0\n",
        "    delete node.child[ch]\n",
        "return FOUND\n"
    };

    enum ANIMATION_STATE{PAUSE, PLAY, REPLAY};
    ANIMATION_STATE animationState; // Trạng thái animation hiện tại
    float timeAnimation = 0.0f; // Thời gian hiện tại của animation
    int curAnimation = 0;   // Chỉ số animation hiện tại
    std::vector<SnapShot> Animation;  //history of animation

    int lineCount;
    Rectangle togglePseudocodeButton;
    bool isPseudocodeVisible;
    void drawPseudocode(const char**, int, int, int);
    void updatePseudocodeOn();

    bool inputActive;    // Flag to check if input is active
    char inputBuffer[20]; // Buffer for user input
    int inputType;       // Type of input (0: None, 1: Create, 2: Search, 3: Insert, 4: Delete, 5: Load file, 6: Clear)

    Color nodeColor = BLACK;
    Color nodeHighlightColor = ORANGE;
    Color nodeDeleteColor = RED;
    Color nodeExistColor = GREEN;
    Color edgeColor = BLACK;
    Color edgeHighlightColor = ORANGE;

    std::vector<button> remote;
    
    std::vector<std::string> listStr;

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



    Trie();
    void init();
    void createRandomTrie(int n);
    void insert(std::string);
    void search(std::string);
    void remove(std::string);
    void update(std::string, std::string);
    void clear();
    void loadFile();
    void handleEvents();
    void handleRemote();
    Vector2 newPosVector2(Vector2, Vector2, float);
    void drawAnimation(SnapShot, float);
    void drawTrie(int);
    void drawButtons();
    void draw();
    ~Trie();
};


#endif // TRIE_H