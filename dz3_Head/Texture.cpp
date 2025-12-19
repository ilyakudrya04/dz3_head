#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
{
    width = 0;
    height = 0;
    channels = 0;
}

bool Texture::load(const std::string& filename)
{
    unsigned char* imageData = stbi_load
    (
        filename.c_str(),
        &width,
        &height,
        &channels,
        3
    );
    if (imageData == nullptr)
    {
        return false;
    }

    int totalPixels = width * height * 3;

    pixels.clear();
    pixels.resize(totalPixels);

    for (int i = 0; i < totalPixels; i++)
    {
        pixels[i] = imageData[i];
    }

    stbi_image_free(imageData);

    return true;
}

ColorRGB Texture::getPixel(float u, float v)
{
    ColorRGB color;
    color.r = 0;
    color.g = 0;
    color.b = 0;

    if (pixels.empty())
    {
        return color;
    }

    while (u > 1.0f)
    {
        u = u - 1.0f;
    }
    while (u < 0.0f)
    {
        u = u + 1.0f;
    }
    
    while (v > 1.0f)
    {
        v = v - 1.0f;
    }
    while (v < 0.0f)
    {
        v = v + 1.0f;
    }

    int x = (int)(u * width);
    int y = (int)((1.0f - v) * height);

    if (x < 0)
    {
        x = 0;
    }
    if (x >= width)
    {
        x = width - 1;
    }

    if (y < 0)
    {
        y = 0;
    }
    if (y >= height)
    {
        y = height - 1;
    }

    int pixelIndex = (y * width + x) * 3;

    color.r = pixels[pixelIndex];
    color.g = pixels[pixelIndex + 1];
    color.b = pixels[pixelIndex + 2];

    return color;
}

unsigned char* Texture::sample(float u, float v)
{
    if (pixels.empty())
    {
        return nullptr;
    }

    while (u > 1.0f)
    {
        u = u - 1.0f;
    }
    while (u < 0.0f)
    {
        u = u + 1.0f;
    }
    
    while (v > 1.0f)
    {
        v = v - 1.0f;
    }
    while (v < 0.0f)
    {
        v = v + 1.0f;
    }

    int x = (int)(u * width);
    int y = (int)((1.0f - v) * height);

    if (x < 0)
    {
        x = 0;
    }
    if (x >= width)
    {
        x = width - 1;
    }

    if (y < 0)
    {
        y = 0;
    }
    if (y >= height)
    {
        y = height - 1;
    }

    int pixelIndex = (y * width + x) * 3;

    return &pixels[pixelIndex];
}

void Texture::free()
{
    pixels.clear();
    width = 0;
    height = 0;
    channels = 0;
}