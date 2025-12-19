#define _CRT_SECURE_NO_WARNINGS
#include "Rasterizer.h"

void Framebuffer::setPixel(int x, int y, Color c, float z, float alpha)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    int index = y * WIDTH + x;

    if (z < depth[index]) {
        depth[index] = z;

        if (alpha >= 1.0f) {
            color[index] = c;
            return;
        }

        if (alpha <= 0.0f) {
            return;
        }

        Color base = color[index];
        Color out;

        out.r = (unsigned char)(base.r * (1.0f - alpha) + c.r * alpha);
        out.g = (unsigned char)(base.g * (1.0f - alpha) + c.g * alpha);
        out.b = (unsigned char)(base.b * (1.0f - alpha) + c.b * alpha);

        color[index] = out;
    }
}

void Framebuffer::savePPM(const char* filename)
{
    FILE* file = fopen(filename, "wb");
    
    if (file == nullptr) {
        return;
    }
    
    fprintf(file, "P6 %d %d 255\n", WIDTH, HEIGHT);

    for (auto& pixel : color) {
        unsigned char rgb[3];
        rgb[0] = pixel.r;
        rgb[1] = pixel.g;
        rgb[2] = pixel.b;
        
        fwrite(rgb, 1, 3, file);
    }
    
    fclose(file);
}

void Framebuffer::clear()
{
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        colorbuffer[i] = Color();
        zbuffer[i] = 999999.0f; // или FLT_MAX
    }
}