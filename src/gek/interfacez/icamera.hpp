#ifndef GEK_ICAMERA_HPP
#define GEK_ICAMERA_HPP

namespace GEK
{

struct iCamera
{
    virtual glm::mat4 getViewMatrix() = 0;
};

}

#endif