#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
using namespace std;

Menu menu;
Tool tool;

void init() {
    menu.init();
    tool.init();   
}

void event() {
    if (current_state == MENU) {
        int x = menu.UpdatePressOn();  
        if (x == 0) {
            current_state = SINGLYLINKLIST;
            return ;
        }
    }

    if (current_state == SINGLYLINKLIST) {
        tool.UpdatePressOn(0);
    }
}

void draw() {
    if (current_state == MENU) {
        draw_bg();
        menu.draw();
    }
    else if (current_state == SINGLYLINKLIST) {
        tool.draw();
    }
}