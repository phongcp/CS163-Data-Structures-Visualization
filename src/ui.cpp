#include <bits/stdc++.h>
#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
using namespace std;

void Menu::init() {
    Button.resize(6);

    Button[0] = {{117,243},{268,240},(Texture2D)LoadTexture("assets/menu_bottom/heaptree.png"),RAYWHITE,1};
    Button[1] = {{575,243},{268,240},(Texture2D)LoadTexture("assets/menu_bottom/avltree.png"),RAYWHITE,1};
    Button[2] = {{1045,243},{268,240},(Texture2D)LoadTexture("assets/menu_bottom/234tree.png"),RAYWHITE,1};
    Button[3] = {{117,523},{268,240},(Texture2D)LoadTexture("assets/menu_bottom/hashtable.png"),RAYWHITE,1};
    Button[4] = {{575,523},{268,240},(Texture2D)LoadTexture("assets/menu_bottom/trie.png"),RAYWHITE,1};
    Button[5] = {{1054,523},{268,240},(Texture2D)LoadTexture("assets/menu_bottom/graph.png"),RAYWHITE,1};

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
        // DrawRectangle(v.Postion.x, v.Postion.y, v.Size.x, v.Size.y, BLUE);
    }
}

int Menu::UpdatePressOn(){
    bool Press = 0;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) Press = 1;  // Kiểm tra click chuột trái
    if (Press) std::cout << Press;
    
    Vector2 x = GetMousePosition();  // Lấy vị trí chuột
    int d = 0;
    // Kiểm tra từng nút
    for (button v :Button) {
        if (v.kind_mouse != 0 && v.CheckPress(x,10,Press)) return d;
        d++;
    }
    return -1;  // Không có nút nào được nhấn
}

void Interface::init(){
    // Khởi tạo mảng Button với 3 phần tử
    Button.resize(3);       
    gtt = -1e8;            // Khởi tạo thời gian với giá trị âm lớn

    // Khởi tạo 3 button chính của giao diện
    Button[0] = button({0,0},{1440,810},              // Background
        LoadTexture("assets/in_app/bg_app.png"),
        RAYWHITE, 0);     

    Button[1] = button({-100,-4},{220,43},            // Logo
        LoadTexture("assets/bg/name.png"),
        RAYWHITE, 1);     

    Button[2] = button({1348,7},{48,48},             // Nút Return
        LoadTexture("assets/in_app/return.png"),
        RAYWHITE, 1);     

    // Khởi tạo thanh điều chỉnh tốc độ
    Speed = button({1111,762},{237,22},              // Thanh tốc độ
        LoadTexture("assets/in_app/speed.png"),
        RAYWHITE, 1);     

    NodeSpeed = button({1188,762},{42,22},           // Nút trượt trên thanh tốc độ
        LoadTexture("assets/in_app/node_speed.png"),
        RAYWHITE, 1);  

    // Điều chỉnh kích thước các texture
    Button[1].image.height /= 3.5;    // Giảm kích thước logo
    Button[1].image.width /= 3.5;

    Speed.image.height /= 4;          // Giảm kích thước thanh tốc độ
    Speed.image.width /= 4;

    NodeSpeed.image.height /= 4;      // Giảm kích thước nút trượt
    NodeSpeed.image.width /= 4;

    // Cập nhật kích thước thực của các button
    Speed.Size.x = Speed.image.width + 2;
    Speed.Size.y = Speed.image.height;
    NodeSpeed.Size.x = NodeSpeed.image.width;
    NodeSpeed.Size.y = NodeSpeed.image.height;

    // Cập nhật kích thước cho tất cả các button chính
    for (button v: Button) {
        v.Size = {(float)v.image.height,(float)v.image.width};
    }

    bool checkNodeSpeed = 0;  // Biến kiểm tra trạng thái của nút trượt
}

Pos pos;
void Interface::draw(){
    Vector2 Mouse = GetMousePosition();  // Lấy vị trí chuột hiện tại
    for (button v : Button) {
        if (v.kind_mouse != 0 && v.CheckMouse(Mouse,3) ) v.col = 1;  // Hiệu ứng khi hover
        else v.col = 0;
        v.DrawBasic(0.7);  // Vẽ nút với độ trong suốt 0.7
    }

    // Chuẩn bị text cho các loại cấu trúc dữ liệu
    const char * A = "Heap Tree";
    const char * B = "AVL Tree";
    const char * C = "234 Tree";
    const char * D = "Trie";
    const char * E = "Hash Table";
    const char * F = "Graph";
    int fontSize = 40;

    // Vẽ tiêu đề tương ứng với cấu trúc dữ liệu đang được chọn
    if (pos == HEAPTREE) {
        Vector2 textSize = MeasureTextEx(customFont, A, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,A,Postion,fontSize,1.0f,WHITE);
    }

    if (pos == HEAPTREE) {
        Vector2 textSize = MeasureTextEx(customFont, A, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,A,Postion,fontSize,1.0f,WHITE);
    }

    if (pos == AVLTREE) {
        Vector2 textSize = MeasureTextEx(customFont, B, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,B,Postion,fontSize,1.0f,WHITE);
    }

    if (pos == TTFTREE) {
        Vector2 textSize = MeasureTextEx(customFont, C, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,C,Postion,fontSize,1.0f,WHITE);
    }

    if (pos == TRIE) {
        Vector2 textSize = MeasureTextEx(customFont, D, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,D,Postion,fontSize,1.0f,WHITE);
    }

    if (pos == HASHTABLE) {
        Vector2 textSize = MeasureTextEx(customFont, E, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,E,Postion,fontSize,1.0f,WHITE);
    }

    if (pos == GRAPH) {
        Vector2 textSize = MeasureTextEx(customFont, F, fontSize, 1);
        Vector2 Postion = {screenWidth/2.0f - textSize.x/2.0f,14};
        DrawTextEx(customFont,F,Postion,fontSize,1.0f,WHITE);
    }
    

    Speed.DrawBasic(1);
    NodeSpeed.DrawBasic(1);
}

int Interface::UpdatePressOn(bool Press){
    int d = 0;
    Vector2 Mouse = GetMousePosition();  // Lấy vị trí chuột

    // Kiểm tra click chuột trên các nút
    for (button v : Button) {
        if (v.kind_mouse != 0 && v.CheckMouse(Mouse,3) && Press) return d + 100;
        d++;
    }

    // Xử lý kéo thả thanh tốc độ
    if (NodeSpeed.CheckMouse(GetMousePosition(),1) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        checkNodeSpeed = 1; 
        if (gtt == -1e8)
            gtt = GetMousePosition().x - NodeSpeed.Postion.x;
      //  std::cout << gtt << "\n";
    }

    // Reset trạng thái khi thả chuột
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        checkNodeSpeed = 0;
        gtt = -1e8;
    }

    // Cập nhật vị trí thanh tốc độ khi kéo
    if (checkNodeSpeed){
        NodeSpeed.Postion.x = GetMousePosition().x - gtt;
           // std::cout << NodeSpeed.Postion.x << "\n";
            if (NodeSpeed.Postion.x < Speed.Postion.x) 
                NodeSpeed.Postion.x = Speed.Postion.x;

            if (NodeSpeed.Postion.x + NodeSpeed.Size.x > Speed.Postion.x + Speed.Size.x) 
                NodeSpeed.Postion.x = Speed.Postion.x + Speed.Size.x - NodeSpeed.Size.x;
          //  std::cout << NodeSpeed.Postion.x << "\n";
    }

    float total = 237/2.0;
    float Nowpos = NodeSpeed.Postion.x + NodeSpeed.Size.x - Speed.Postion.x;
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