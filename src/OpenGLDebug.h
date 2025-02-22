//
// Created by Steven Bayer on 2/11/25.
//

#ifndef OPENGLDEBUG_H
#define OPENGLDEBUG_H

#include <iostream>
#include <GLFW/glfw3.h>

namespace debug {
inline void drawDebugAxes() {
    glBegin(GL_LINES);
    // X axis - Red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);

    // Y axis - Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    // Z axis - Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
}

inline void debugViewport() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    std::cout << "Viewport: x=" << viewport[0]
              << " y=" << viewport[1]
              << " width=" << viewport[2]
              << " height=" << viewport[3] << std::endl;
}

inline void debugMatrices() {
    GLfloat modelview[16];
    GLfloat projection[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    glGetFloatv(GL_PROJECTION_MATRIX, projection);

    std::cout << "Modelview Matrix:\n";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << modelview[i * 4 + j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Projection Matrix:\n";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << projection[i * 4 + j] << " ";
        }
        std::cout << "\n";
    }
}

inline void drawBoundingBox(const float size = 1.0f) {
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);  // White
    float s = size/2;
    // Front face
    glVertex3f(-s, -s, s);
    glVertex3f(s, -s, s);
    glVertex3f(s, s, s);
    glVertex3f(-s, s, s);
    glEnd();
    // Add other faces similarly
}

inline void debugTransform(const Transform& transform) {
    std::cout << "Position: "
              << transform.position.x << ", "
              << transform.position.y << ", "
              << transform.position.z << "\n";
    std::cout << "Scale: "
              << transform.scale.x << ", "
              << transform.scale.y << ", "
              << transform.scale.z << "\n";
}

inline void checkGLErrors(const char* label) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << label << ": "
                  << std::hex << error << std::dec << std::endl;
    }
}

}

#endif //OPENGLDEBUG_H
