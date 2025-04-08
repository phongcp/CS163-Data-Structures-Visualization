#include "..\include\node.h"

Node::Node(std::string value, Vector2 pos, Color col, float r, Font f, float fSize, Vector2 vec)
    : data(value), position(pos), color(col), radius(r), font(f), fontSize(fSize), velocity(vec) {}

void Node::draw(int rad, int ft) {
    // DrawCircleV(position, radius, color);
    DrawRing(position, radius - 5, radius, 0, 360, 100, color);
    
    // Center the text in the circle
    Vector2 textSize = MeasureTextEx(font, data.c_str(), fontSize, 1);
    Vector2 textPos = {position.x - textSize.x / 2, position.y - textSize.y / 2};
    DrawTextEx(font, data.c_str(), textPos, fontSize, 1, color);
}