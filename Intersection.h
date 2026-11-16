//
// Created by capit on 16.11.2026.
//

#ifndef RAYTRACING_INTERSECTION_H
#define RAYTRACING_INTERSECTION_H
#include <tuple>

#include "Vector.h"
#include "Material.h"
#include "Scene.h"

// ==========================================
// INTERSECTIONS
// ==========================================

inline bool intersect_bounding_sphere(
    const vec3& orig,
    const vec3& dir,
    const BoundingSphere& bs
) {
    vec3 L = bs.center - orig;
    float tca = L * dir;
    float d2 = L * L - tca * tca;
    if (d2 > bs.radius * bs.radius)
        return false;
    float thc = std::sqrt(bs.radius * bs.radius - d2);
    return (tca + thc) > 0.001f;
}

inline bool is_inside_bounds(const vec3& p, const Bounds3D& b) {
    if (!b.enabled) return true;
    return p.x >= b.min_p.x && p.x <= b.max_p.x &&
           p.y >= b.min_p.y && p.y <= b.max_p.y &&
           p.z >= b.min_p.z && p.z <= b.max_p.z;
}

inline std::tuple<bool, float> ray_quadric_intersect(
    const vec3& orig,
    const vec3& dir,
    const QuadricSurface& surf
) {
    if (surf.bbox.enabled && !intersect_bounding_sphere(orig, dir, surf.bbox))
        return {false, 0.f};

    vec3 Adir = surf.A * dir;
    vec3 Aorig = surf.A * orig;

    float a = dir * Adir;
    float b = 2.f * (orig * Adir) + (surf.B * dir);
    float c = (orig * Aorig) + (surf.B * orig) + surf.C;

    if (std::abs(a) < 1e-6f) {
        if (std::abs(b) < 1e-6f) return {false, 0.f};
        float t = -c / b;
        if (t > 0.001f && is_inside_bounds(orig + dir * t, surf.clip))
            return {true, t};
        return {false, 0.f};
    }

    float discriminant = b * b - 4.f * a * c;
    if (discriminant < 0.f)
        return {false, 0.f};

    float sqrtd = std::sqrt(discriminant);
    float inv_2a = 0.5f / a;
    float t0 = (-b - sqrtd) * inv_2a;
    float t1 = (-b + sqrtd) * inv_2a;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 > 0.001f && is_inside_bounds(orig + dir * t0, surf.clip))
        return {true, t0};
    if (t1 > 0.001f && is_inside_bounds(orig + dir * t1, surf.clip))
        return {true, t1};

    return {false, 0.f};
}

inline vec3 quadric_normal(const vec3& p, const QuadricSurface& surf) {
    vec3 n = (surf.A * p) * 2.f + surf.B;
    return n.normalized();
}

struct Hit {
    bool found = false;
    vec3 point;
    vec3 normal;
    RTMaterial material;
};

inline Hit scene_intersect(const vec3& orig, const vec3& dir) {
    Hit result;
    float nearest_dist = 1e10f;

    if (std::abs(dir.y) > 0.001f) {
        float d = -(orig.y + 4.f) / dir.y;
        vec3 p = orig + dir * d;

        if (
            d > 0.001f &&
            d < nearest_dist &&
            std::abs(p.x) < 40.f &&
            p.z < 60.f &&
            p.z > -100.f
        ) {
            nearest_dist = d;
            result.found = true;
            result.point = p;
            result.normal = {0.f, 1.f, 0.f};

            result.material.diffuse_color =
                (int(0.5f * p.x + 1000) + int(0.5f * p.z)) & 1
                ? vec3{0.4f, 0.25f, 0.3f}
                : vec3{0.15f, 0.25f, 0.55f};

            result.material.albedo[0] = 1.f;
        }
    }

    for (const QuadricSurface& obj : objects) {
        auto [intersection, d] = ray_quadric_intersect(orig, dir, obj);

        if (!intersection || d > nearest_dist)
            continue;

        nearest_dist = d;
        result.found = true;
        result.point = orig + dir * d;
        result.normal = quadric_normal(result.point, obj);

        if (result.normal * dir > 0.f) {
            result.normal = -result.normal;
        }

        result.material = obj.material;
    }

    return result;
}



#endif //RAYTRACING_INTERSECTION_H
