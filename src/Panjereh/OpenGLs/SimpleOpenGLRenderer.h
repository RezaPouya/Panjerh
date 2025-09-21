#pragma once

class SimpleOpenGLRenderer {
public:
    // Very simple functions for beginners
    static void DrawRedBackground();
    static void DrawBlueSquare();
    static void DrawGreenCircle();
    static void DrawRainbow();

    // Clear screen with any color
    static void ClearScreen(float r, float g, float b);
};