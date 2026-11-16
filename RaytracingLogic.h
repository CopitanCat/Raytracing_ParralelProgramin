//
// Created by capit on 16.11.2026.
//

#ifndef RAYTRACING_RAYTRACINGLOGIC_H
#define RAYTRACING_RAYTRACINGLOGIC_H
#include "Vector.h"
#include "Intersection.h"
#include <algorithm>


// ==========================================
// RAY TRACING MATH
// ==========================================

vec3 reflect(const vec3& I, const vec3& N) {
    return I - N * (2.f * (I * N));
}

vec3 refract(
    const vec3& I,
    const vec3& N,
    float eta_t,
    float eta_i = 1.f
) {
    float cosi = -std::clamp(I * N, -1.f, 1.f);

    if (cosi < 0.f)
        return refract(I, -N, eta_i, eta_t);

    float eta = eta_i / eta_t;
    float k = 1.f - eta * eta * (1.f - cosi * cosi);

    if (k < 0.f)
        return {0.f, 0.f, 0.f};

    return I * eta + N * (eta * cosi - std::sqrt(k));
}

// ==========================================
// SHADOWS
// ==========================================

bool is_in_shadow(const vec3& point, const vec3& normal, const vec3& light) {
    constexpr float bias = 0.001f;

    vec3 to_light = light - point;
    const float light_distance = to_light.norm();
    if (light_distance <= bias)
        return false;

    const vec3 light_dir = to_light * (1.f / light_distance);
    const float side = (light_dir * normal) >= 0.f ? 1.f : -1.f;
    vec3 ray_origin = point + normal * (side * bias);
    float travelled = 0.f;

    for (int step = 0; step < 16; ++step) {
        Hit blocker = scene_intersect(ray_origin, light_dir);
        if (!blocker.found)
            return false;

        const float hit_distance = (blocker.point - ray_origin).norm();
        if (travelled + hit_distance >= light_distance - bias)
            return false;

        const bool emissive = blocker.material.emission.norm() > 0.01f;
        const bool refractive = blocker.material.albedo[3] > 0.01f;
        if (!emissive && !refractive)
            return true;

        const float advance = hit_distance + bias;
        ray_origin = ray_origin + light_dir * advance;
        travelled += advance;
    }

    return true;
}

// ==========================================
// RAY TRACER (с динамической глубиной)
// ==========================================

vec3 cast_ray(
    const vec3& orig,
    const vec3& dir,
    int max_depth,
    int depth = 0
) {
    Hit hit = scene_intersect(orig, dir);

    if (depth > max_depth || !hit.found)
        return {0.2f, 0.7f, 0.8f};

    const RTMaterial& material = hit.material;

    vec3 reflect_color = {0.f, 0.f, 0.f};
    vec3 refract_color = {0.f, 0.f, 0.f};

    // Reflection
    if (material.albedo[2] > 0.f) {
        vec3 reflect_dir = reflect(dir, hit.normal).normalized();
        vec3 reflect_orig =
            hit.point + hit.normal *
            (reflect_dir * hit.normal < 0.f ? -0.001f : 0.001f);

        reflect_color = cast_ray(reflect_orig, reflect_dir, max_depth, depth + 1);
    }

    // Refraction
    if (material.albedo[3] > 0.f) {
        vec3 refract_dir =
            refract(dir, hit.normal, material.refractive_index).normalized();

        if (refract_dir.norm() > 0.f) {
            vec3 refract_orig =
                hit.point + hit.normal *
                (refract_dir * hit.normal < 0.f ? -0.001f : 0.001f);

            refract_color = cast_ray(refract_orig, refract_dir, max_depth, depth + 1);
        }
    }

    float diffuse_light_intensity = 0.12f;
    float specular_light_intensity = 0.f;

    for (const vec3& light : lights) {
        vec3 light_dir = (light - hit.point).normalized();

        if (is_in_shadow(hit.point, hit.normal, light))
            continue;

        diffuse_light_intensity += std::max(0.f, light_dir * hit.normal);

        if (material.albedo[1] > 0.f && material.specular_exponent > 0.f) {
            vec3 ref_light = reflect(-light_dir, hit.normal);
            specular_light_intensity += std::pow(
                std::max(0.f, ref_light * (-dir)),
                material.specular_exponent
            );
        }
    }

    return
        material.emission +
        material.diffuse_color * diffuse_light_intensity * material.albedo[0] +
        vec3{1.f, 1.f, 1.f} * specular_light_intensity * material.albedo[1] +
        reflect_color * material.albedo[2] +
        refract_color * material.albedo[3];
}

#endif //RAYTRACING_RAYTRACINGLOGIC_H
