#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

struct ColorRGB
{
    int r;
    int g;
    int b;
};

class Texture
{
public:
    int width;
    int height;
    int channels;

    std::vector<unsigned char> pixels;

    Texture();

    bool load(const std::string& filename);

    ColorRGB getPixel(float u, float v);

    unsigned char* sample(float u, float v);

    void free();
};

#endif