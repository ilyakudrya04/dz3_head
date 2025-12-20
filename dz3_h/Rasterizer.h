#ifndef RASTERIZER_H
#define RASTERIZER_H


#pragma once
#include <vector>
#include <cstdio>
#include <algorithm>
#include "Math.h"
#include "Shader.h"

static int WIDTH = 800;
static int HEIGHT = 800;

class Framebuffer
{
public:

    std::vector<Color> color;
    std::vector<float> depth;
    Color* colorbuffer;
    float* zbuffer;

    Framebuffer()
   {
        color.resize(WIDTH * HEIGHT);
        depth.resize(WIDTH * HEIGHT, 1e9);
        colorbuffer = new Color[WIDTH * HEIGHT];
        zbuffer = new float[WIDTH * HEIGHT];
        clear();
    }


    void setPixel(int x, int y, Color c, float z, float alpha = 1.0f);
    void clear();
    void savePPM(const char* filename);


};

inline Math::Vec3 bary(float px, float py, const Math::Vec3& A, const Math::Vec3& B, const Math::Vec3& C)
{
    using namespace Math;

    Vec3 v0 = B - A;
    Vec3 v1 = C - A;
    Vec3 v2(px - A.x, py - A.y, 0);

    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);

    float denom = d00 * d11 - d01 * d01;

    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    
    float u = 1.0f - v - w;

    return Vec3(u, v, w);
}

inline void drawLine(Framebuffer& fb, int x0, int y0, int x1, int y1, Color c) {
    bool steep = false;

    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = std::abs(y1 - y0);

    int err = dx / 2;
    
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {
            if (y >= 0 && y < WIDTH && x >= 0 && x < HEIGHT)
                fb.setPixel(y, x, c, -99999);
        } else {
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
                fb.setPixel(x, y, c, -99999);
        }

        err -= dy;
        
        if (err < 0) {
            y += ystep;
            err += dx;
        }
    }
}


#endif //RASTERIZER_H
