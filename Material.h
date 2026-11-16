//
// Created by capit on 16.11.2026.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H
#include "Surface.h"

// ==========================================
// MATERIALS AND QUADRIC OBJECTS
// ==========================================



constexpr RTMaterial ivory = {
    1.0f, {0.9f, 0.5f, 0.1f, 0.0f},
    {0.4f, 0.4f, 0.3f}, 50.f
};

constexpr RTMaterial glass = {
    1.15f, {0.0f, 0.9f, 0.1f, 0.8f},
    {0.6f, 0.7f, 0.8f}, 250.f
};

constexpr RTMaterial red_rubber = {
    1.0f, {1.4f, 0.3f, 0.0f, 0.0f},
    {0.3f, 0.1f, 0.1f}, 10.f
};

constexpr RTMaterial mirror = {
    1.0f, {0.0f, 16.0f, 0.8f, 0.0f},
    {1.0f, 1.0f, 1.0f}, 1425.f
};

constexpr RTMaterial lamp = {
    1.0f, {0.f, 0.f, 0.f, 0.f},
    {0.f, 0.f, 0.f}, 0.f,
    {8.f, 6.f, 3.f}
};

//Описание квадратов через куб
constexpr RTMaterial cube_red     = { 1.0f, {1.0f, 0.5f, 0.0f, 0.0f}, {0.9f, 0.1f, 0.1f}, 50.f };
constexpr RTMaterial cube_green   = { 1.0f, {1.0f, 0.5f, 0.0f, 0.0f}, {0.1f, 0.9f, 0.1f}, 50.f };
constexpr RTMaterial cube_blue    = { 1.0f, {1.0f, 0.5f, 0.0f, 0.0f}, {0.1f, 0.1f, 0.9f}, 50.f };
constexpr RTMaterial cube_yellow  = { 1.0f, {1.0f, 0.5f, 0.0f, 0.0f}, {0.9f, 0.9f, 0.1f}, 50.f };
constexpr RTMaterial cube_cyan    = { 1.0f, {1.0f, 0.5f, 0.0f, 0.0f}, {0.1f, 0.9f, 0.9f}, 50.f };
constexpr RTMaterial cube_magenta = { 1.0f, {1.0f, 0.5f, 0.0f, 0.0f}, {0.9f, 0.1f, 0.9f}, 50.f };

#endif //RAYTRACING_MATERIAL_H
