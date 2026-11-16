//
// Created by capit on 16.11.2026.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H
#include <cmath>
#include <algorithm>
#include "Vector.h"
#include <raylib.h>


// ==========================================
// CAMERA
// ==========================================

struct RTCamera3D {
    vec3 position = {0.f, 0.f, 0.f};
    float yaw = 0.f;
    float pitch = 0.f;

    vec3 forward() const {
        float cp = std::cos(pitch);
        return {
            std::sin(yaw) * cp,
            std::sin(pitch),
            -std::cos(yaw) * cp
        };
    }

    vec3 right() const {
        return cross(forward(), {0.f, 1.f, 0.f}).normalized();
    }

    vec3 up() const {
        return cross(right(), forward()).normalized();
    }

    bool update(float dt) {
        float speed = 6.f;
        if (IsKeyDown(KEY_LEFT_SHIFT))
            speed *= 3.f;

        vec3 f = forward();
        vec3 r = right();
        bool moved = false;

        if (IsKeyDown(KEY_W)) { position = position + f * speed * dt; moved = true; }
        if (IsKeyDown(KEY_S)) { position = position - f * speed * dt; moved = true; }
        if (IsKeyDown(KEY_D)) { position = position + r * speed * dt; moved = true; }
        if (IsKeyDown(KEY_A)) { position = position - r * speed * dt; moved = true; }
        if (IsKeyDown(KEY_SPACE)) { position.y += speed * dt; moved = true; }
        if (IsKeyDown(KEY_LEFT_CONTROL)) { position.y -= speed * dt; moved = true; }

        Vector2 mouse = GetMouseDelta();
        if (std::abs(mouse.x) > 0.01f || std::abs(mouse.y) > 0.01f) {
            float sensitivity = 0.0025f;
            yaw += mouse.x * sensitivity;
            pitch -= mouse.y * sensitivity;
            pitch = std::clamp(pitch, -1.4f, 1.4f);
            moved = true;
        }

        return moved;
    }
};



#endif //RAYTRACING_CAMERA_H
