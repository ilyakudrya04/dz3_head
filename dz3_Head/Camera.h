#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include "Math.h"

class Camera 
{
public:
    Math::Vec3 position;
    Math::Vec3 target;
    Math::Vec3 up;

    Camera(Math::Vec3 pos, Math::Vec3 tgt)
        : position(pos), target(tgt), up(0,1,0) {}

    Math::Mat4 getView() {
        return Math::lookAt(position, target, up);
    }

    Math::Mat4 getProjection(float fov, float aspect) {
        return Math::perspective(fov, aspect, 0.1f, 100.0f);
    }
};



#endif //CAMERA_H
