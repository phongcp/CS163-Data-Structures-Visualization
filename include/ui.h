#ifndef UI_H
#define UI_H

#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
using namespace std;

void draw_bg();

class Menu {
public:
    void init();
    void draw();
    int UpdatePressOn();

private:
    vector <button> Button;
};

class Tool {
private:
    vector <button> Button;     // Danh sách các nút điều khiển
    button SpeedBar, SpeedNode;          // Nút điều chỉnh tốc độ và tốc độ node
    bool checkSpeedNode;                    // Trạng thái kiểm tra tốc độ node
    int gtt;                        // Giá trị thời gian

public:
    void init();                           // Khởi tạo giao diện
    void draw();                           // Vẽ giao diện
    int UpdatePressOn(bool Press);         // Xử lý sự kiện nhấn nút
};

#endif