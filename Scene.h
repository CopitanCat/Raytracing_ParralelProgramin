//
// Created by capit on 16.11.2026.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H
#include <cmath>
#include "Material.h"

// ===================================
// Objects and their position
// ===================================

const QuadricSurface objects[] = {
    QuadricSurface::make_sphere({-3.f, 0.f, -16.f}, 2.f, ivory),
    QuadricSurface::make_sphere({-1.0f, -1.5f, -12.f}, 2.f, glass),
    QuadricSurface::make_ellipsoid({1.5f, -0.5f, -18.f}, {3.5f, 2.2f, 2.5f}, red_rubber),
    QuadricSurface::make_sphere({7.f, 5.f, -18.f}, 4.f, mirror),
    QuadricSurface::make_cylinder_y({-6.5f, -1.5f, -14.f}, 1.0f, 5.0f, ivory),

    // --- ЛАМПЫ ---
    QuadricSurface::make_sphere({-20.f, 10.f, -20.f}, 0.4f, lamp),
    QuadricSurface::make_sphere({0.f, 12.f, -20.f}, 0.4f, lamp),
    QuadricSurface::make_sphere({10.f, 8.f, -10.f}, 0.4f, lamp),

    // 1. Правая грань (Красная) [X = 17.0]
    {
                {{{0,0,0},{0,0,0},{0,0,0}}}, {1.f, 0.f, 0.f}, -17.0f,
                cube_blue, {}, {{16.9f, -2.f, -24.f}, {17.1f, 2.f, -20.f}, true}
    },
    // 2. Левая грань (Зеленая) [X = 13.0]
    {
                {{{0,0,0},{0,0,0},{0,0,0}}}, {-1.f, 0.f, 0.f}, 13.0f,
                cube_blue, {}, {{12.9f, -2.f, -24.f}, {13.1f, 2.f, -20.f}, true}
    },
    // 3. Верхняя грань (Синяя) [Y = 2.0]
    {
                {{{0,0,0},{0,0,0},{0,0,0}}}, {0.f, 1.f, 0.f}, -2.0f,
                cube_blue, {}, {{13.f, 1.9f, -24.f}, {17.f, 2.1f, -20.f}, true}
    },
    // 4. Нижняя грань (Желтая) [Y = -2.0]
    {
                {{{0,0,0},{0,0,0},{0,0,0}}}, {0.f, -1.f, 0.f}, -2.0f,
                cube_blue, {}, {{13.f, -2.1f, -24.f}, {17.f, -1.9f, -20.f}, true}
    },
    // 5. Передняя грань (Голубая) [Z = -24.0]
    {
                {{{0,0,0},{0,0,0},{0,0,0}}}, {0.f, 0.f, -1.f}, -24.0f,
                cube_blue, {}, {{13.f, -2.f, -24.1f}, {17.f, 2.f, -23.9f}, true}
    },
    // 6. Задняя грань (Пурпурная) [Z = -20.0]
    {
                {{{0,0,0},{0,0,0},{0,0,0}}}, {0.f, 0.f, 1.f}, 20.0f,
                cube_blue, {}, {{13.f, -2.f, -20.1f}, {17.f, 2.f, -19.9f}, true}
    }
};

// lights position

constexpr vec3 lights[] = {
    {-20.f, 10.f, -20.f},
    {0.f, 12.f, -20.f},
    {10.f, 8.f, -10.f}
};


#endif //RAYTRACING_SCENE_H
