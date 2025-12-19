#include "Math.h"

namespace Math {

    Vec3 cross(const Vec3& a, const Vec3& b)
    {
        return Vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    float dot(const Vec3& a, const Vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vec3 normalize(const Vec3& v)
{

        float length = std::sqrt(dot(v, v));
        return Vec3(v.x / length, v.y / length, v.z / length);
    }

    Vec4 mul(const Mat4& m, const Vec4& v)
    {
        Vec4 result;
        
        result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0];
        result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1];
        result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2];
        result.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3];
        
        return result;
    }

    Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up) {
        Vec3 forward = center - eye;
        forward = normalize(forward);

        Vec3 right = cross(forward, up);
        right = normalize(right);

        Vec3 upVector = cross(right, forward);

        Mat4 viewMatrix = {};
        
        viewMatrix.m[0][0] = right.x;
        viewMatrix.m[1][0] = right.y;
        viewMatrix.m[2][0] = right.z;
        
        viewMatrix.m[0][1] = upVector.x;
        viewMatrix.m[1][1] = upVector.y;
        viewMatrix.m[2][1] = upVector.z;
        
        viewMatrix.m[0][2] = -forward.x;
        viewMatrix.m[1][2] = -forward.y;
        viewMatrix.m[2][2] = -forward.z;
        
        viewMatrix.m[3][0] = -dot(right, eye);
        viewMatrix.m[3][1] = -dot(upVector, eye);
        viewMatrix.m[3][2] = dot(forward, eye);
        viewMatrix.m[3][3] = 1.0f;

        return viewMatrix;
    }

    Mat4 perspective(float fov, float aspect, float zn, float zf) {
        Mat4 projectionMatrix = {};

        float tanHalfFov = std::tan(fov * 0.5f);

        projectionMatrix.m[0][0] = 1.0f / (aspect * tanHalfFov);
        projectionMatrix.m[1][1] = 1.0f / tanHalfFov;
        projectionMatrix.m[2][2] = -(zf + zn) / (zf - zn);
        projectionMatrix.m[2][3] = -1.0f;
        projectionMatrix.m[3][2] = -(2.0f * zf * zn) / (zf - zn);
        projectionMatrix.m[3][3] = 0.0f;

        return projectionMatrix;
    }

}