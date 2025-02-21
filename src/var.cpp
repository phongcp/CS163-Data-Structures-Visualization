#include <raylib.h>
#include "../include/var.h"

const int screenWidth = 1280;
const int screenHeight = 720;

double deltaTime = 0.5f;  // Thời gian giữa các frame
Font customFont = GetFontDefault();

// Kiểm tra vị trí chuột có nằm trong button không
bool button::CheckMouse(Vector2 A,int k){
    return Postion.x <= A.x - k && Postion.x + Size.x >= A.x + k 
        && Postion.y <= A.y - k && Postion.y + Size.y >= A.y + k;  
}

// Kiểm tra sự kiện nhấn chuột trên button
bool button::CheckPress(Vector2 A,int k,bool _press){
    press = 0;
    if (!_press) return false;
    if (CheckMouse(A,k)) {
        press = 1;
        return true;
    }
    return false;
}

// Vẽ button với hiệu ứng fade   
void button::DrawBasic(float g){
    float k = 1;
    if (col && kind_mouse != 5) k = g;
    DrawTexture(image,Postion.x,Postion.y,Fade(color,k));
}

void draw_bg(){
    DrawRectangle(0,0,screenWidth,screenHeight, Color {150,140,228,255});
    // DrawTexture(Logo,0,0,RAYWHITE);
}