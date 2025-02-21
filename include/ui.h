#ifndef UI_H
#define UI_H

#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
using namespace std;

class Menu {
public:
    void init();
    void draw();
    int UpdatePressOn();

private:
    vector <button> Button;
};

class Interface {
private:
    vector <button> Button;     // Danh sách các nút điều khiển
    button Speed, NodeSpeed;          // Nút điều chỉnh tốc độ và tốc độ node
    bool checkNodeSpeed;                    // Trạng thái kiểm tra tốc độ node
    int gtt;                        // Giá trị thời gian

public:
    void init();                           // Khởi tạo giao diện
    void draw();                           // Vẽ giao diện
    int UpdatePressOn(bool Press);         // Xử lý sự kiện nhấn nút
};

#endif