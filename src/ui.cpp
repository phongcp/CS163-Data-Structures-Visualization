#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
using namespace std;

void draw_bg(){
    DrawRectangle(0,0,screenWidth,screenHeight, Color {80, 80, 80, 255});
    DrawTexture(Logo,0,0,RAYWHITE);
}

void Menu::init() {
    Button.resize(6);

    Button[0] = {{117,243},{1,1},(Texture2D)LoadTexture("../assets/menu_bottom/SinglyLinkList.png"),RAYWHITE,1};
    Button[1] = {{575,243},{1,1},(Texture2D)LoadTexture("../assets/menu_bottom/avltree.png"),RAYWHITE,1};
    Button[2] = {{1045,243},{1,1},(Texture2D)LoadTexture("../assets/menu_bottom/heap.png"),RAYWHITE,1};
    Button[3] = {{117,523},{1,1},(Texture2D)LoadTexture("../assets/menu_bottom/hashtable.png"),RAYWHITE,1};
    Button[4] = {{575,523},{1,1},(Texture2D)LoadTexture("../assets/menu_bottom/trie.png"),RAYWHITE,1};
    Button[5] = {{1054,523},{1,1},(Texture2D)LoadTexture("../assets/menu_bottom/graph.png"),RAYWHITE,1};

    for (button &v: Button) v.Size = {(float)v.image.width,(float)v.image.height};
}

// Vẽ menu và xử lý hiệu ứng hover
void Menu::draw(){
    Vector2 Mouse = GetMousePosition();
    for (button v : Button) {
        // Kiểm tra hover
        if (v.CheckMouse(Mouse,10)) v.col = 1;
        else v.col = 0;
        // Vẽ nút với hiệu ứng fade
        v.DrawBasic(0.6);
    }
}

int Menu::UpdatePressOn(){
    bool Press = 0;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) Press = 1;  // Kiểm tra click chuột trái
    if (Press) cout << Press;
    
    Vector2 x = GetMousePosition();  // Lấy vị trí chuột
    int d = 0;
    // Kiểm tra từng nút
    for (button v :Button) {
        if (v.kind_mouse != 0 && v.CheckPress(x,10,Press)) return d;
        d++;
    }
    return -1;  // Không có nút nào được nhấn
}

void Tool::init(){
    musicBackGround = LoadMusicStream("../assets/music/BackGroundMusic.ogg"); // Tải nhạc nền
    PlayMusicStream(musicBackGround);

    // Khởi tạo mảng Button với 3 phần tử
    Button.resize(3);       
    gtt = -1e8;            // Khởi tạo thời gian với giá trị âm lớn

    // Khởi tạo 3 button chính của giao diện
    // Button[0] = button({0,0},{1440,810},              // Background
    //     LoadTexture("../assets/in_app/bg_app.png"),
    //     RAYWHITE, 0);     

    Button[1] = button({-100,-4},{220,43},            // Logo
        LoadTexture("../assets/bg/name.png"),
        RAYWHITE, 1);     

    Button[2] = button({1348,7},{48,48},             // Nút Return
        LoadTexture("../assets/in_app/return.png"),
        RAYWHITE, 1);     
        
    // Light mode and dark mode
    lightMode = button({1200,7},{102,48},              // Nút Light mode
        LoadTexture("../assets/in_app/light_mode.png"),
        RAYWHITE, 1);
    darkMode = button({1200,7},{102,48},              // Nút Dark mode
        LoadTexture("../assets/in_app/dark_mode.png"),
        RAYWHITE, 1);
    lightMode.image.height /= 5;          // Giảm kích thước nút Light mode
    lightMode.image.width /= 5;

    darkMode.image.height /= 5;          // Giảm kích thước nút Dark mode
    darkMode.image.width /= 5;

    // mute and unmute
    muteButton = button({1120,0},{51.2,51.2},              // Nút Mute
        LoadTexture("../assets/in_app/mute.png"),
        RAYWHITE, 1);
    unmuteButton = button({1120,0},{51.2,51.2},              // Nút Unmute
        LoadTexture("../assets/in_app/unmute.png"),
        RAYWHITE, 1);
    muteButton.image.height /= 5;          // Giảm kích thước nút Mute
    unmuteButton.image.height /= 5;
    muteButton.image.width /= 5;          // Giảm kích thước nút Mute
    unmuteButton.image.width /= 5;          // Giảm kích thước nút Unmute

    // Khởi tạo thanh điều chỉnh tốc độ
    SpeedBar = button({92, 762},{237,22},              // Thanh tốc độ
        LoadTexture("../assets/in_app/speed.png"),
        RAYWHITE, 1);     

    SpeedNode = button({210, 762},{42,22},           // Nút trượt trên thanh tốc độ
        LoadTexture("../assets/in_app/node_speed.png"),
        RAYWHITE, 1);  

    // Điều chỉnh kích thước các texture
    Button[1].image.height /= 3.5;    // Giảm kích thước logo
    Button[1].image.width /= 3.5;

    SpeedBar.image.height /= 4;          // Giảm kích thước thanh tốc độ
    SpeedBar.image.width /= 4;

    SpeedNode.image.height /= 4;      // Giảm kích thước nút trượt
    SpeedNode.image.width /= 4;

    // Cập nhật kích thước thực của các button
    SpeedBar.Size.x = SpeedBar.image.width + 2;
    SpeedBar.Size.y = SpeedBar.image.height;
    SpeedNode.Size.x = SpeedNode.image.width;
    SpeedNode.Size.y = SpeedNode.image.height;

    // Cập nhật kích thước cho tất cả các button chính
    for (button v: Button) {
        v.Size = {(float)v.image.height,(float)v.image.width};
    }

    bool checkSpeedNode = 0;  // Biến kiểm tra trạng thái của nút trượt
}

void Tool::draw(){
    Vector2 Mouse = GetMousePosition();  // Lấy vị trí chuột hiện tại
    for (button v : Button) {
        if (v.kind_mouse != 0 && v.CheckMouse(Mouse,3) ) v.col = 1;  // Hiệu ứng khi hover
        else v.col = 0;
        v.DrawBasic(0.7);  // Vẽ nút với độ trong suốt 0.7
    }

    // Draw FPS
    if(current_state != MENU)DrawTextGradient(TextFormat("FPS: %i", GetFPS()), 10, 70, 20, RED, YELLOW, 1); // draw FPS

    // Chuẩn bị text cho các loại cấu trúc dữ liệu
    int fontSize = 40;
    
    // Vẽ tiêu đề tương ứng với cấu trúc dữ liệu đang được chọn
    if (current_state == SINGLYLINKLIST) {
        const char * A = "Singly Linked List";
        Vector2 textSize = MeasureTextEx(customFont, A, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,A,Postion,fontSize,1.0f,WHITE);
    }
    else if (current_state == HEAPTREE) {
        const char * A = "Heap Tree";
        Vector2 textSize = MeasureTextEx(customFont, A, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,A,Postion,fontSize,1.0f,WHITE);
    }
    else if (current_state == AVLTREE) {
        const char * A = "AVL Tree";
        Vector2 textSize = MeasureTextEx(customFont, A, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,A,Postion,fontSize,1.0f,WHITE);
    }
    else if (current_state == HASHTABLE) {
        const char * A = "Hash Table";
        Vector2 textSize = MeasureTextEx(customFont, A, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,A,Postion,fontSize,1.0f,WHITE);
    }
    else if (current_state == TRIE) {
        const char * A = "Trie";
        Vector2 textSize = MeasureTextEx(customFont, A, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,A,Postion,fontSize,1.0f,WHITE);
    }
    else if (current_state == GRAPH) {
        const char * A = "Graph";
        Vector2 textSize = MeasureTextEx(customFont, A, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,A,Postion,fontSize,1.0f,WHITE);
    }
    
    if(isLightMode) lightMode.DrawBasic(1); 
    else darkMode.DrawBasic(1);   
    if(isPlaySong) unmuteButton.DrawBasic(1); 
    else muteButton.DrawBasic(1);
    SpeedBar.DrawBasic(1);
    SpeedNode.DrawBasic(1);
}

int Tool::UpdatePressOn(){
    bool Press = 0;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) Press = 1;

    int d = 0;
    Vector2 Mouse = GetMousePosition();  // Lấy vị trí chuột

    // Kiểm tra click chuột trên nút Light mode
    if (lightMode.CheckMouse(GetMousePosition(), 1) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        isLightMode = !isLightMode;  // Chuyển sang chế độ sáng
    }

    UpdateMusicStream(musicBackGround);
    if (muteButton.CheckMouse(GetMousePosition(), 1) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        isPlaySong = !isPlaySong;  // Chuyển sang chế độ tắt tiếng
        if(isPlaySong) ResumeMusicStream(musicBackGround); // Tiếp tục phát nhạc
        else PauseMusicStream(musicBackGround); // Tạm dừng nhạc
    }

    // Kiểm tra click chuột trên các nút
    for (button v : Button) {
        if (v.kind_mouse != 0 && v.CheckMouse(Mouse,3) && Press) return d + 100;
        d++;
    }


    // Xử lý kéo thả thanh tốc độ
    if (SpeedNode.CheckMouse(GetMousePosition(),1) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        checkSpeedNode = 1; 
        if (gtt == -1e8)
            gtt = GetMousePosition().x - SpeedNode.Postion.x;
    }

    // Reset trạng thái khi thả chuột
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        checkSpeedNode = 0;
        gtt = -1e8;
    }

    // Cập nhật vị trí thanh tốc độ khi kéo
    if (checkSpeedNode){
        SpeedNode.Postion.x = GetMousePosition().x - gtt;
            if (SpeedNode.Postion.x < SpeedBar.Postion.x) 
                SpeedNode.Postion.x = SpeedBar.Postion.x;

            if (SpeedNode.Postion.x + SpeedNode.Size.x > SpeedBar.Postion.x + SpeedBar.Size.x) 
                SpeedNode.Postion.x = SpeedBar.Postion.x + SpeedBar.Size.x - SpeedNode.Size.x;
    }

    float total = 237/2.0;
    float Nowpos = SpeedNode.Postion.x + SpeedNode.Size.x - SpeedBar.Postion.x;
    if (Nowpos <= total) {
        float x = Nowpos / (total / 10.0);
        deltaTime = 0.25/(0.1*x);
    }
    else {
        Nowpos -= total;
        float x = Nowpos / (total /20.0);
        deltaTime = 0.25/(1.0 + 1.0*x);
    }
    
    return -1;  // Không có nút nào được nhấn
}