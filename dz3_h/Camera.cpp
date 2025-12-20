#include "Camera.h"

Camera::Camera(Math::Vec3 pos, Math::Vec3 tgt)
    : position(pos), target(tgt), up(Math::Vec3(0, 1, 0)) {
}

Math::Mat4 Camera::getView() const {
    return Math::lookAt(position, target, up);
}

Math::Mat4 Camera::getProjection(float fov, float aspect) const {
    return Math::perspective(fov, aspect, 0.1f, 100.0f);
}