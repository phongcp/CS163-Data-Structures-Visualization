#include "..\include\node.h"
#include "..\include\var.h"

Node::Node() {
    data = "null";
    position = {0, 0};
    color = BLACK;
    radius = 30.0f;
    font = customFont;
    fontSize = 20.0f;
    velocity = {0, 0};
    isDragging = false;
}

Node::Node(std::string value, Vector2 pos, Color col, float r, Font f, float fSize, Vector2 vec)
    : data(value), position(pos), color(col), radius(r), font(f), fontSize(fSize), velocity(vec) {}

void Node::draw(int rad, int ft) {
    DrawRing(position, radius - 5, radius, 0, 360, 100, color);
    
    Vector2 textSize = MeasureTextEx(font, data.c_str(), fontSize, 1);
    Vector2 textPos = {position.x - textSize.x / 2, position.y - textSize.y / 2};
    DrawTextEx(font, data.c_str(), textPos, fontSize, 1, color);
}