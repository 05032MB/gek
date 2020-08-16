#ifndef GEK_ICAMERA_HPP
#define GEK_ICAMERA_HPP

namespace GEK
{

struct iCamera
{
    virtual glm::mat4 getViewMatrix() = 0;
    virtual glm::vec3 getPosition() = 0;
};

struct iCameraCanMoveWithKbd : public iCamera
{
    enum movement
    {
        forwards, backwards, lefts, rights, ups, downs, pitchups, pitchdowns, yawups, yawdowns, rollups, rolldowns,
    };
    virtual void moveWithKbd(movement dir, float deltaTime) = 0;
};

struct iCameraCanMoveWithMouse : public iCamera
{
    virtual void moveWithMouse(float xoffset, float yoffset) = 0;
};

struct iCameraStandardOps : public iCameraCanMoveWithKbd, public iCameraCanMoveWithMouse
{

};

}

#endif