#include "SimpleOpenGLRenderer.h"
#include <GLFW/glfw3.h>
#include <cmath>

void SimpleOpenGLRenderer::DrawRedBackground() {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red
    glClear(GL_COLOR_BUFFER_BIT);
}

void SimpleOpenGLRenderer::DrawBlueSquare() {
    // Set blue color
    glColor3f(0.0f, 0.0f, 1.0f); // Blue

    // Draw a square
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f); // Bottom-left
    glVertex2f(0.5f, -0.5f); // Bottom-right
    glVertex2f(0.5f, 0.5f); // Top-right
    glVertex2f(-0.5f, 0.5f); // Top-left
    glEnd();
}

void SimpleOpenGLRenderer::DrawGreenCircle() {
    // Set green color
    glColor3f(0.0f, 1.0f, 0.0f); // Green

    // Draw a circle using triangles
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); // Center

    int segments = 32;
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * float(i) / float(segments);
        glVertex2f(cos(angle) * 0.5f, sin(angle) * 0.5f);
    }
    glEnd();
}

void SimpleOpenGLRenderer::DrawRainbow() {
    // Draw multiple colored squares
    float colors[][3] = {
        {1.0f, 0.0f, 0.0f}, // Red
        {1.0f, 0.5f, 0.0f}, // Orange
        {1.0f, 1.0f, 0.0f}, // Yellow
        {0.0f, 1.0f, 0.0f}, // Green
        {0.0f, 0.0f, 1.0f}, // Blue
        {0.5f, 0.0f, 1.0f}  // Purple
    };

    for (int i = 0; i < 6; i++) {
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);

        glBegin(GL_QUADS);
        float size = 0.8f - (i * 0.1f);
        glVertex2f(-size, -size);
        glVertex2f(size, -size);
        glVertex2f(size, size);
        glVertex2f(-size, size);
        glEnd();
    }
}

void SimpleOpenGLRenderer::ClearScreen(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}