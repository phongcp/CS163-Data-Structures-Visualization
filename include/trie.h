#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include "var.h"
#include "node.h"
#include "tinyfiledialogs.h"


class Trie{
private:

    struct TrieNode{
        Node* node;
        TrieNode* parent;
        TrieNode* children[26];
        bool isEndOfWord;

        TrieNode(){
            isEndOfWord = false;
            node = nullptr;
            parent = nullptr;
            for(int i = 0; i < 26; ++i){
                children[i] = nullptr;
            }
        }
    };

    float generatePosition(TrieNode* u, float t, float f, float h, float delh, float r);
    void adjustPosition();

    // Pseudocode for hash function
    const char* insertCode[6] = {
        "node = root\n",
        "for(char ch : word)\n"
        "    if node.child[ch] == NULL\n",
        "        node.child[ch] = new TrieNode(ch)\n",
        "    node = node.child[ch]\n",
        "node.isEndOfWord = true\n",
    };

    const char* searchCode[8] = {
        "node = root\n",
        "for(char ch : word)\n",
        "    if node.child[ch] == NULL\n",
        "        return NOT_FOUND\n",
        "    cur.numberChild++\n",
        "    cur = cur.child[ch]\n",
        "if cur.isEndOfWord return FOUND\n",
        "return NOT_FOUND\n",
    };

    const char* deleteCode[8] = {
        "if (search(word) == NOT_FOUND) return NOT_FOUND\n",
        "node = endOfWordNode(word)\n"
        "for(char ch : rev(word))\n",
        "    node.numChild--\n"
        "    node = node.parent\n",
        "    if node.child[ch].numChild == 0\n",
        "        delete node.child[ch]\n",
        "return FOUND\n"
    };

    enum ANIMATION_STATE{PAUSE, PLAY, REPLAY};
    ANIMATION_STATE animationState; // Trạng thái animation hiện tại
    float timeAnimation = 0.0f; // Thời gian hiện tại của animation
    int curAnimation = 0;   // Chỉ số animation hiện tại
    std::vector<int> Animation;  //history of animation

    int lineCount;
    Rectangle togglePseudocodeButton;
    bool isPseudocodeVisible;
    void drawPseudocode();
    void updatePseudocodeOn();

    int capacity;

    bool inputActive;    // Flag to check if input is active
    char inputBuffer[20]; // Buffer for user input
    int inputType;       // Type of input (0: None, 1: Create, 2: Search, 3: Insert, 4: Delete, 5: Load file, 6: Clear)

    std::vector<button> remote;
    
    std::vector<std::string> listStr;

    ButtonText addButton; // Button to add a new key-value pair
    ButtonText searchButton; // Button to search for a value by key
    ButtonText deleteButton; // Button to delete a key-value pair
    ButtonText createButton; // Button to create a random hash table
    ButtonText fileButton; // Button to handle file operations
    ButtonText clearButton; // Button to clear the hash table

    TrieNode* root; // Root of the trie
public:
    Trie();
    void init();
    void createRandomTrie();
    void deleteNode(TrieNode*);
    void insert(std::string);
    void search(std::string);
    void remove(std::string);
    void clear();
    void loadFile();
    void createRandomString(int len);
    void handleEvents();
    void handleRemote();
    void drawTrie(TrieNode*);
    void drawButtons();
    void draw();
    ~Trie();
};


#endif // TRIE_H