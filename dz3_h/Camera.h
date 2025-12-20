#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include "Math.h"

#pragma once
#include "Math.h"

class Camera {
private:
    Math::Vec3 position;
    Math::Vec3 target;
    Math::Vec3 up;
    float fov;
    float aspect;

public:
    Camera(Math::Vec3 pos, Math::Vec3 tgt);

    Math::Mat4 getView() const;
    Math::Mat4 getProjection(float fov, float aspect) const; 
};


#endif //CAMERA_H
