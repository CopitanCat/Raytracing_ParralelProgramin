//
// Created by capit on 16.11.2026.
//

#ifndef RAYTRACING_VECTOR_H
#define RAYTRACING_VECTOR_H
#include <cmath>
// ==========================================
// VECTOR & MATRIX 3x3
// ==========================================

struct vec3 {
    float x = 0, y = 0, z = 0;

    float& operator[](int i) {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    const float& operator[](int i) const {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    vec3 operator*(float v) const {
        return {x * v, y * v, z * v};
    }

    float operator*(const vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    vec3 operator+(const vec3& v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    vec3 operator-(const vec3& v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    vec3 operator-() const {
        return {-x, -y, -z};
    }

    float norm() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    vec3 normalized() const {
        float n = norm();
        return n > 1e-8f ? *this * (1.f / n) : vec3{};
    }
};

vec3 cross(const vec3& a, const vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

struct mat3 {
    float m[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    vec3 operator*(const vec3& v) const {
        return {
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        };
    }
};

#endif //RAYTRACING_VECTOR_H
