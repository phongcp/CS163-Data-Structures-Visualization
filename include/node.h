#ifndef NODE_H
#define NODE_H

#include <string>
#include <raylib.h>

class Node {
public:
    std::string data;
    Vector2 position;
    Color color;
    float radius;
    Font font;
    float fontSize;
    Vector2 velocity;
    bool isDragging = false;

    Node();
    Node(std::string value, Vector2 pos, Color col, float r, Font f, float fSize, Vector2 vec={0,0});

    void draw(int rad=0, int ft=0);
};

#endif