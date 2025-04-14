#ifndef AVLTREE_H
#define AVLTREE_H


#include <string>
#include <vector>
#include "var.h"
#include "manager.h"
#include "tinyfiledialogs.h"


class AVLTree {
private:


    // Pseudocode for hash function
    const char* insertValueCode[6] = {
        "Insert v\n",
        "Check balance factor (bf):\n",
        "    LL: rt_Right\n",
        "    LR: left.rt_Left, rt_Right\n",
        "    RR: rt_Left\n",
        "    RL: right.rt_Right, rt_Left\n"
    };

    const char* searchValueCode[6] = {
        "if (this == NULL) return NOT_FOUND\n",
        "if (this.data == value) return FOUND\n",
        "if (value < this.data) searchLeft\n",
        "if (value > this.data) searchRight\n",
    };

    const char* deleteValueCode[6] = {
        "Delete v",
        "Check balance factor (bf):\n",
        "    LL: rt_Right\n",
        "    LR: left.rt_Left, rt_Right\n",
        "    RR: rt_Left\n",
        "    RL: right.rt_Right, rt_Left\n"
    };

    enum ANIMATION_STATE{PAUSE, PLAY, REPLAY};
    ANIMATION_STATE animationState; // Trạng thái animation hiện tại

    int lineCount;
    Rectangle togglePseudocodeButton;
    bool isPseudocodeVisible;
    void drawPseudocode(const char**, int, int);
    void updatePseudocodeOn();


    char inputBuffer[5];
    bool inputActive;
    int inputType;

    std::vector<int> Animation;
    float timeAnimation = 0.0f; // Thời gian hiện tại của animation
    int curAnimation = 0;   // Chỉ số animation hiện tại

    std::vector<button> remote;

    ButtonText addButton;
    ButtonText searchButton;
    ButtonText deleteButton; 
    ButtonText createButton; 
    ButtonText fileButton; 
    ButtonText clearButton;

public:
    AVLTree();
    void init();
    void loadFile();
    void clear();
    void handleRemote();
    void handleEvents();
    void draw();
    void drawButtons();
    ~AVLTree();
};



#endif // AVLTREE_H