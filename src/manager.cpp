#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
#include "../include/SinglyLinkList.h"
using namespace std;

Menu menu;
Tool tool;
SinglyLinkedList singlyLinkedList;

void init() {
    menu.init();
    tool.init();
    singlyLinkedList.init();
}

void event() {
    if (current_state == MENU) {
        int x = menu.UpdatePressOn();  
        if (x == 0) {
            current_state = SINGLYLINKLIST;
            return;
        }
    }

    if (current_state == SINGLYLINKLIST) {
        singlyLinkedList.handleEvents(); // Handle events for singly linked list
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
}