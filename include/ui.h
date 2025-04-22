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
    ~Menu(){
        for (button v: Button) v.unLoad();
    }

private:
    vector <button> Button;
};

class Tool {
private:
    vector <button> Button;     // Danh sách các nút điều khiển
    button SpeedBar, SpeedNode;          // Nút điều chỉnh tốc độ và tốc độ node
    bool checkSpeedNode;                    // Trạng thái kiểm tra tốc độ node
    int gtt;                        // Giá trị thời gian
    Music musicBackGround;

public:
    button lightMode, darkMode, muteButton, unmuteButton;
    void init();                           // Khởi tạo giao diện
    void draw();                           // Vẽ giao diện
    int UpdatePressOn();         // Xử lý sự kiện nhấn nút
    ~Tool(){
        for (button v: Button) v.unLoad();
        lightMode.unLoad(); 
        darkMode.unLoad();
        muteButton.unLoad();
        unmuteButton.unLoad();
        SpeedBar.unLoad();
        SpeedNode.unLoad();
        UnloadMusicStream(musicBackGround); // Giải phóng nhạc nền
    }
};

#endif