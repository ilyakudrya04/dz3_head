#ifndef MATH_H
#define MATH_H

#pragma once
#include <cmath>

namespace Math {

    struct Vec2 {
        float x;
        float y;

        Vec2() : x(0), y(0) {}
        
        Vec2(float X, float Y) : x(X), y(Y) {}

        Vec2 operator+(const Vec2& o) const { return Vec2(x + o.x, y + o.y); }
        
        Vec2 operator-(const Vec2& o) const { return Vec2(x - o.x, y - o.y); }
        
        Vec2 operator*(float s) const { return Vec2(x * s, y * s); }
    };

    struct Vec3 {
        float x;
        float y;
        float z;

        Vec3() : x(0), y(0), z(0) {}
        
        Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

        Vec3 operator+(const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
        
        Vec3 operator-(const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
        
        Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    };

    struct Vec4 {
        float x;
        float y;
        float z;
        float w;
    };

    struct Mat4 {
        float m[4][4]{};
    };

    Vec3 cross(const Vec3& a, const Vec3& b);

    float dot(const Vec3& a, const Vec3& b);

    Vec3 normalize(const Vec3& v);

    Vec4 mul(const Mat4& m, const Vec4& v);

    Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up);

    Mat4 perspective(float fov, float aspect, float zn, float zf);

}

#endif //MATH_H
