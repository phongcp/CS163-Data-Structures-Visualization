#ifndef VAR_H
#define VAR_H

#include <bits/stdc++.h>
#include <raylib.h>

extern const int screenWidth;
extern const int screenHeight;

extern Font customFont;
extern Font fontPseudocode;
extern Texture2D Logo;
extern double deltaTime;

extern bool isLightMode;
extern bool isPlaySong;

extern float radius;
extern float fontSize;

enum CurST{
    MENU,       // Menu chính
    SINGLYLINKLIST,     // Singly Link List
    MSTree,   // Heap
    AVLTREE,    // Cây AVL
    HASHTABLE,  // Bảng băm
    TRIE,       // Cây Trie
    SHORTESTPATH      // Đồ thị
};

extern CurST current_state;

// Kiểm tra xem màu sắc có giống nhau hay không
inline bool ColorsAreEqual(Color c1, Color c2) {
    return (c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c2.b) && (c1.a == c2.a);
}

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

struct Edge {
    int nodeA, nodeB, Weight;
    float restLength;
    Color color;
    bool isSelected = false;
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

struct ButtonText{
    Rectangle bounds;
    Color defaultColor;
    Color hoverColor;
    Color clickColor;
    Color currentColor;
    const char *text;
    float roundness;
    int segments;

    ButtonText(){};

    ButtonText(float x, float y, float width, float height, const char *label) {
        bounds = {x, y, width, height};
        defaultColor = DARKGRAY;
        hoverColor = GRAY;
        clickColor = LIGHTGRAY;
        currentColor = defaultColor;
        text = label;
        roundness = 0.3f;
        segments = 10;
    }
    
    bool IsHovered(Vector2 mousePoint) {
        return CheckCollisionPointRec(mousePoint, bounds);
    }

    bool IsClicked(Vector2 mousePoint) {
        return IsHovered(mousePoint) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    void Update() {
        Vector2 mousePoint = GetMousePosition();
        if (IsHovered(mousePoint)) {
            currentColor = (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) ? clickColor : hoverColor;
        } else {
            currentColor = defaultColor;
        }
    }

    void Draw() {
        DrawRectangleRounded(bounds, roundness, segments, currentColor);
        DrawRectangleRoundedLines(bounds, roundness, segments, BLACK);

        int textWidth = MeasureText(text, 17);
        float textX = bounds.x + (bounds.width - textWidth) / 2;
        float textY = bounds.y + (bounds.height - 17) / 2;

        // DrawText(text, textX, textY, 17, WHITE);
        DrawTextEx(customFont, text, {textX, textY}, 17, 1.0f, WHITE); // Draw the text in the button
    }
};



class InputTextBox {
private:
    Rectangle bounds;
    std::string text;
    std::string prefix;
    bool isActive;
    int maxLength;
    float fontSize;
    Color borderColor;
    Color textColor;
    Color backgroundColor;

public:
    InputTextBox(){};
    InputTextBox(Rectangle rect, std::string prefixStr = "", int maxLen = 32, float fontSize = 20.0f,
                    Color borderColor = BLACK, Color textColor = BLACK, Color bgColor = RAYWHITE)
        : bounds(rect), prefix(prefixStr), maxLength(maxLen), fontSize(fontSize),
            borderColor(borderColor), textColor(textColor), backgroundColor(bgColor),
            isActive(false) {}
    
    void createRandomValue(int l, int r){
        int randomValue = GetRandomValue(l, r);
        text = std::to_string(randomValue);
    }

    void createRandomString(int len){
        text.clear();
        for(int i = 0; i < len; ++i){
            text += (char)(GetRandomValue(0, 25) + 'A'); // Generate a random uppercase letter
        }
    }

    void reGenerateString(){ // for Alphabet 'A' to 'Z'
        for(int i = 0; i < text.size(); ++i){
            if('A' <= text[i] && text[i] <= 'Z') continue;
            if('a' <= text[i] && text[i] <= 'z') text[i] = text[i] - 32;
            else{
                text.erase(i, 1);
                --i;
            }
        }
    }

    void Update() {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            bool wasActive = isActive;
            isActive = CheckCollisionPointRec(mousePos, bounds);
        }

        if (isActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (text.length() < maxLength)) {
                    text += static_cast<char>(key);
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !text.empty()) {
                text.pop_back();
            }
        }
    }

    void Draw() const {
        DrawRectangleRec(bounds, backgroundColor);
        DrawRectangleLinesEx(bounds, 2, borderColor);

        float prefixWidth = MeasureText(prefix.c_str(), fontSize);
        float yOffset = bounds.y + (bounds.height - fontSize) / 2;

        // Draw prefix
        DrawText(prefix.c_str(), bounds.x + 5, yOffset, fontSize, textColor);

        // Draw text
        DrawText(text.c_str(), bounds.x + 5 + prefixWidth, yOffset, fontSize, textColor);

        // Draw cursor
        if (isActive && ((GetTime() * 2.0f) - static_cast<int>(GetTime() * 2.0f) < 0.5f)) {
            float textWidth = MeasureText(text.c_str(), fontSize);
            DrawText("_", bounds.x + 5 + prefixWidth + textWidth + 2, yOffset, fontSize, textColor);
        }
    }

    int GetIntValue() const {
        return std::stoi(text);
    }

    std::string GetText() const {
        return text;
    }

    void SetText(const std::string& newText) {
        text = newText.substr(0, maxLength);
    }

    void Clear() {
        text.clear();
    }

    void SetActive(bool active) {
        isActive = active;
    }

    bool IsActive() const {
        return isActive;
    }

    void SetPrefix(const std::string& newPrefix) {
        prefix = newPrefix;
    }

    std::string GetPrefix() const {
        return prefix;
    }
};



struct HSLColor {
    float h;        // Hue (0-360)
    float s;        // Saturation (0-1)
    float l;        // Lightness (0-1)
};

HSLColor ColorToHSL(Color color);
Color HSLToColor(HSLColor hsl);
Color ConvertToDarkMode(Color original);


long long Rand(long long l, long long r);
Vector2 GetCircleEdgePoint(Vector2 center1, Vector2 center2, float radius);

void DrawTextGradient(const char *, int, int, int, Color, Color, int);
void init_bg();
void draw_bg();
void DrawConnection(Vector2 p1, Vector2 p2, bool direct=0, Color color=BLACK, float thickness=5, float r1=radius, float r2=radius);


#endif 