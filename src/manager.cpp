#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
#include "../include/SinglyLinkedList.h"
#include "../include/manager.h"
#include "../include/hashtable.h"
#include "../include/trie.h"
#include "../include/avltree.h"

using namespace std;

Menu menu;
Tool tool;
SinglyLinkedList singlyLinkedList;
HashTable hashTable;
Trie trie;
AVLTree avlTree;


void init() {
    init_bg();
    menu.init();
    tool.init();
    singlyLinkedList.init();
    hashTable.init();
    trie.init();
    avlTree.init();
}

void event() {
    if (current_state == MENU) {
        int x = menu.UpdatePressOn();  
        if (x == 0) {
            current_state = SINGLYLINKLIST;
            return;
        }
        else if(x == 1) {
            current_state = AVLTREE;
            return;
        }
        else if(x == 2) {
            current_state = HEAPTREE;
            return;
        }
        else if(x == 3) {
            current_state = HASHTABLE;
            return;
        }
        else if(x == 4) {
            current_state = TRIE;
            return;
        }
        else if(x == 5) {
            current_state = GRAPH;
            return;
        }   
    }

    if (current_state == SINGLYLINKLIST) {
        singlyLinkedList.handleEvents(); // Handle events for singly linked list
    }
    else if(current_state == HASHTABLE){
        hashTable.handleEvents(); // Handle events for hash table
    }
    else if(current_state == TRIE){
        trie.handleEvents(); // Handle events for trie
    }
    else if(current_state == AVLTREE){
        avlTree.handleEvents(); // Handle events for AVL tree
    }

    int x = tool.UpdatePressOn();
    if (x == 102) {
        current_state = MENU;
    }
}

void draw() {
    if (current_state == MENU) {
        draw_bg();
        if(isLightMode) tool.lightMode.DrawBasic(1);
        else tool.darkMode.DrawBasic(1);
        menu.draw();
    }
    else {
        tool.draw();   // Draw tool (speed bar, speed node, title)
        if (current_state == SINGLYLINKLIST) {            
            singlyLinkedList.draw();
            singlyLinkedList.drawButtons(); // Draw buttons for singly linked list
        }
        if (current_state == HASHTABLE) {
            hashTable.draw();
            hashTable.drawButtons(); // Draw buttons for hash table
        }
        if (current_state == TRIE){
            trie.draw();
        }
        if (current_state == AVLTREE){
            avlTree.draw();
        }
    }
}