#include <raylib.h>
#include "../include/var.h"

const int screenWidth = 1440;
const int screenHeight = 810;

double deltaTime = 0.5f;  // Thời gian giữa các frame
Font customFont;
Font fontPseudocode;
Texture2D Logo;

bool isLightMode = true;
bool isPlaySong = true;

float radius = 25; 
float fontSize = 15;

CurST current_state;

void init_bg() {
    Logo = LoadTexture("../assets/bg/name.png");
    
    customFont = LoadFontEx("../assets/fonts/Roboto/Roboto-Bold.ttf",100,nullptr,0);
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_POINT);

    fontPseudocode = LoadFontEx("../assets/fonts/Cour/courbd.ttf", 20, nullptr, 0);
    SetTextureFilter(fontPseudocode.texture, TEXTURE_FILTER_POINT);
}


long long Rand(long long l, long long r){
    long long res = 0;
    for (int i = 0; i < 4; i ++) res = (res << 15) ^ rand();
    return l + res % (r - l + 1);
}

void DrawTextGradient(const char *text, int posX, int posY, int fontSize, Color colorStart, Color colorEnd, int spacing)
{
    int totalWidth = MeasureText(text, fontSize) + spacing * (TextLength(text) - 1);
    int currentX = posX;

    for (int i = 0; text[i] != '\0'; i++)
    {
        char letter[2] = { text[i], '\0' };
        float factor = (float)(currentX - posX) / totalWidth;
        Color letterColor = ColorLerp(colorStart, colorEnd, factor);

        int letterWidth = MeasureText(letter, fontSize);
        DrawText(letter, currentX, posY, fontSize, letterColor);
        currentX += letterWidth + spacing;  // Add spacing between characters
    }
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

// Convert RGB color to HSL
HSLColor ColorToHSL(Color color) {
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;

    float max = std::max({r, g, b});
    float min = std::min({r, g, b});
    float delta = max - min;

    HSLColor hsl;
    hsl.h = 0;
    hsl.s = 0;
    hsl.l = (max + min) / 2.0f;

    if (delta > 0.0001f) {
        hsl.s = hsl.l < 0.5f ? delta / (max + min) : delta / (2.0f - max - min);
        
        if (max == r) {
            hsl.h = (g - b) / delta + (g < b ? 6.0f : 0.0f);
        }
        else if (max == g) {
            hsl.h = (b - r) / delta + 2.0f;
        }
        else {
            hsl.h = (r - g) / delta + 4.0f;
        }
        
        hsl.h *= 60.0f;
        if (hsl.h < 0) hsl.h += 360.0f;
    }

    return hsl;
}

// Convert HSL color to RGB
Color HSLToColor(HSLColor hsl) {
    float c = (1 - fabs(2 * hsl.l - 1)) * hsl.s;
    float x = c * (1 - fabs(fmod(hsl.h / 60.0f, 2) - 1));
    float m = hsl.l - c / 2.0f;

    float r, g, b;
    
    if (hsl.h < 60)      { r = c; g = x; b = 0; }
    else if (hsl.h < 120) { r = x; g = c; b = 0; }
    else if (hsl.h < 180) { r = 0; g = c; b = x; }
    else if (hsl.h < 240) { r = 0; g = x; b = c; }
    else if (hsl.h < 300) { r = x; g = 0; b = c; }
    else                  { r = c; g = 0; b = x; }

    return Color {
        static_cast<unsigned char>((r + m) * 255),
        static_cast<unsigned char>((g + m) * 255),
        static_cast<unsigned char>((b + m) * 255),
        255
    };
}

// Convert color to dark mode version
Color ConvertToDarkMode(Color original) {
    HSLColor hsl = ColorToHSL(original);
    
    // Apply dark mode transformations
    hsl.l *= 0.5f;                  // Reduce lightness
    hsl.s = std::min(hsl.s * 1.2f, 1.0f); // Slightly increase saturation
    hsl.h = fmod(hsl.h + 10.0f, 360.0f);  // Cool tone shift (optional)
    
    Color result = HSLToColor(hsl);
    result.a = original.a; // Preserve alpha channel
    
    return result;
}