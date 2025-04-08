#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
#include "../include/SinglyLinkedList.h"
#include "../include/mst.h"
#include "../include/shortestpath.h"
using namespace std;

Menu menu;
Tool tool;
SinglyLinkedList singlyLinkedList;
ShortestPath shortestPath; // Declare shortestPath object
MST mst; 

void init() {
    menu.init();
    tool.init();
    singlyLinkedList.init();
    shortestPath.init();
    mst.init(); 
}

void event() {
    if (current_state == MENU) {
        int x = menu.UpdatePressOn();  
        if (x == 0) {
            current_state = SINGLYLINKLIST;
            return;
        }
        if (x == 4) {
            current_state = SHORTESTPATH; // Switch to singly linked list state
            return;
        }
        if (x == 5) {
            current_state = MSTree; // Switch to MST state
            return;
        }
    }

    if (current_state == SINGLYLINKLIST) {
        singlyLinkedList.handleEvents(); // Handle events for singly linked list
    }
    else if (current_state == SHORTESTPATH) {
        shortestPath.handleEvents(); // Handle events for shortest path
    }
    else if (current_state == MSTree) {
        mst.handleEvents(); // Handle events for MST
    }

    int x = tool.UpdatePressOn();
    if (x == 102) {
        current_state = MENU;
    }
}

void draw() {
    if (current_state == MENU) {
        draw_bg();
        menu.draw();
    }
    else if (current_state == SINGLYLINKLIST) {
        tool.draw();
        singlyLinkedList.draw();
        singlyLinkedList.drawButtons(); // Draw buttons for singly linked list
    }
    else if (current_state == SHORTESTPATH) {
        tool.draw();
        shortestPath.draw();
    }
    else if (current_state == MSTree) {
        tool.draw();
        mst.draw(); // Draw the MST
    }
}