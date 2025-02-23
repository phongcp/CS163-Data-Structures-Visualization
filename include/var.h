#ifndef VAR_H
#define VAR_H

#include <bits/stdc++.h>
#include <raylib.h>

extern const int screenWidth;
extern const int screenHeight;

extern Font customFont;
extern double deltaTime;

enum CurST{
    MENU,       // Menu chính
    SINGLYLINKLIST,     // Singly Link List
    HEAPTREE,   // Heap
    AVLTREE,    // Cây AVL
    HASHTABLE,  // Bảng băm
    TRIE,       // Cây Trie
    GRAPH      // Đồ thị
};

extern CurST current_state;

// Enum định nghĩa các loại thao tác
enum Kind{
    Create,     // Tạo mới
    Insert,     // Thêm
    Delete,     // Xóa
    Extract,    // Trích xuất
    Update,     // Cập nhật
    Search,     // Tìm kiếm
    file        // Xử lý file
};

struct button {
    Vector2 Postion;    // Vị trí nút
    Vector2 Size;       // Kích thước nút
    Texture2D image;    // Hình ảnh nút
    Color color;        // Màu sắc
    int kind_mouse;     // Loại tương tác chuột
    bool col;           // Trạng thái màu
    bool press;         // Trạng thái nhấn

    button() : Postion({0,0}), Size({0,0}), image({}), color(RAYWHITE), 
               kind_mouse(0), col(false), press(false) {}

    button(Vector2 pos, Vector2 size, Texture2D img, Color clr, int kind) {
        Postion = pos;
        Size = size;
        kind_mouse = kind;
        image = img;
        color = clr;
        col = false;
        press = false;
    }
    
    void DrawBasic(float g);                    // Vẽ nút cơ bản
    bool CheckMouse(Vector2 A,int k);           // Kiểm tra vị trí chuột
    bool CheckPress(Vector2 A,int k,bool _press); // Kiểm tra nhấn chuột
};

void draw_bg();

#endif 