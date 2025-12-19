#ifndef SHADER_H
#define SHADER_H
#pragma once
#include <algorithm>
#include "Math.h"
#include "Texture.h"

using namespace Math;

struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

inline Color phongTexture(const Vec3& normal, const Vec3& fragPos, unsigned char* tex)
{
    if (tex == nullptr)
    {
        Color errorColor;
        errorColor.r = 255;
        errorColor.g = 0;
        errorColor.b = 255;
        return errorColor;
    }

    Vec3 N = normalize(normal);

    Vec3 lightPos(3.0f, 3.0f, 3.0f);

    Vec3 lightDirection = lightPos - fragPos;
    Vec3 L = normalize(lightDirection);

    float dotProduct = dot(N, L);
    
    float diffuse = dotProduct;
    if (diffuse < 0.0f)
    {
        diffuse = 0.0f;
    }

    float intensity = 0.2f + diffuse * 0.8f;
    
    if (intensity > 1.0f)
    {
        intensity = 1.0f;
    }

    int red = (int)(tex[0] * intensity);
    int green = (int)(tex[1] * intensity);
    int blue = (int)(tex[2] * intensity);

    if (red > 255) red = 255;
    if (green > 255) green = 255;
    if (blue > 255) blue = 255;

    Color finalColor;
    finalColor.r = (unsigned char)red;
    finalColor.g = (unsigned char)green;
    finalColor.b = (unsigned char)blue;
    
    return finalColor;
}
#endif //SHADER_H