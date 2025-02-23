#ifndef MANAGER_H
#define MANAGER_H

#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
using namespace std;

extern Menu menu;
extern Tool tool;
void init();
void event();
void draw();

#endif