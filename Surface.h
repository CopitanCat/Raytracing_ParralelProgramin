//
// Created by capit on 16.11.2026.
//

#ifndef RAYTRACING_SURFACE_H
#define RAYTRACING_SURFACE_H
#include "Vector.h"
#include <cmath>
#include <algorithm>


// ====================================
// Surfaces
// ====================================

struct RTMaterial {
    float refractive_index = 1.f;
    float albedo[4] = {2.f, 0.f, 0.f, 0.f};
    vec3 diffuse_color = {0.f, 0.f, 0.f};
    float specular_exponent = 0.f;
    vec3 emission = {0.f, 0.f, 0.f};
};

struct BoundingSphere {
    vec3 center = {0.f, 0.f, 0.f};
    float radius = 0.f;
    bool enabled = false;
};

struct Bounds3D {
    vec3 min_p = {-1e5f, -1e5f, -1e5f};
    vec3 max_p = { 1e5f,  1e5f,  1e5f};
    bool enabled = false;
};

struct QuadricSurface {
    mat3 A;
    vec3 B;
    float C;
    RTMaterial material;
    BoundingSphere bbox;
    Bounds3D clip;

    static constexpr QuadricSurface make_sphere(const vec3& c, float r, const RTMaterial& mat) {
        return {
            {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}},
            {-2.f * c.x, -2.f * c.y, -2.f * c.z},
            (c.x * c.x + c.y * c.y + c.z * c.z) - r * r,
            mat,
            {c, r + 0.05f, true},
            {}
        };
    }

    static constexpr QuadricSurface make_ellipsoid(const vec3& c, const vec3& radii, const RTMaterial& mat) {
        float ix = 1.f / (radii.x * radii.x);
        float iy = 1.f / (radii.y * radii.y);
        float iz = 1.f / (radii.z * radii.z);
        float max_r = std::max({radii.x, radii.y, radii.z});
        return {
            {{{ix, 0, 0}, {0, iy, 0}, {0, 0, iz}}},
            {-2.f * c.x * ix, -2.f * c.y * iy, -2.f * c.z * iz},
            (c.x * c.x * ix + c.y * c.y * iy + c.z * c.z * iz) - 1.f,
            mat,
            {c, max_r + 0.05f, true},
            {}
        };
    }

    static QuadricSurface make_cylinder_y(const vec3& c, float r, float height, const RTMaterial& mat) {
        float half_h = height * 0.5f;
        float bound_r = std::sqrt(r * r + half_h * half_h);
        return {
            {{{1, 0, 0}, {0, 0, 0}, {0, 0, 1}}},
            {-2.f * c.x, 0.f, -2.f * c.z},
            (c.x * c.x + c.z * c.z) - r * r,
            mat,
            {c, bound_r + 0.05f, true},
            {{-1e5f, c.y - half_h, -1e5f}, {1e5f, c.y + half_h, 1e5f}, true}
        };
    }
};
#endif //RAYTRACING_SURFACE_H
