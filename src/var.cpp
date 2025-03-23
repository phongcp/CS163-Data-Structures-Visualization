#include <raylib.h>
#include "../include/var.h"

const int screenWidth = 1440;
const int screenHeight = 810;

double deltaTime = 0.5f;  // Thời gian giữa các frame
Font customFont;
Texture2D Logo;

float radius = 25; 
float fontSize = 15;

CurST current_state;

void init_bg() {
    Logo = LoadTexture("../assets/bg/name.png");
    
    customFont = LoadFontEx("../assets/fonts/Roboto/Roboto-Bold.ttf",100,nullptr,0);
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_POINT);
}

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

Vector2 GetCircleEdgePoint(Vector2 center1, Vector2 center2, float radius) {
    float angle = atan2(center2.y - center1.y, center2.x - center1.x);
    return { (float)(center1.x + radius * cos(angle)), (float)(center1.y + radius * sin(angle)) };
}

// Hàm vẽ đường nối giữa hai hình tròn
void DrawConnection(Vector2 p1, Vector2 p2, bool direct,  Color color, float thickness, float r1, float r2) {
    Vector2 start = GetCircleEdgePoint(p1, p2, r1);
    Vector2 end = GetCircleEdgePoint(p2, p1, r2);
    DrawLineEx(start, end, thickness, color);

    if (!direct) return;

    float angle = atan2(end.y - start.y, end.x - start.x);
    float arrowSize = 15;  // Giảm kích thước mũi tên

    Vector2 left = { 
        end.x - (float)(arrowSize * cos(angle - PI / 5)), 
        end.y - (float)(arrowSize * sin(angle - PI / 5)) 
    };
    
    Vector2 right = { 
        end.x - (float)(arrowSize * cos(angle + PI / 5)), 
        end.y - (float)(arrowSize * sin(angle + PI / 5)) 
    };

    DrawLineEx(end, left, thickness-1, color);
    DrawLineEx(end, right, thickness-1, color);
}