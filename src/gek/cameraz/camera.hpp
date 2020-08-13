#ifndef GEK_CAMERA_HPP
#define GEK_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gek/except.hpp>
#include <gek/interfacez/icamera.hpp>

namespace GEK
{

class camera : public iCamera
{
    glm::vec3 position, antidirection, positivx, up, absoluteUp;

    float yaw, pitch; //nie czuje sie na siłach na kwaterniony

    void updateCameraVectors()
    {
        glm::vec3 antidirection;
        antidirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        antidirection.y = sin(glm::radians(pitch));
        antidirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->antidirection = glm::normalize(antidirection);

        this->positivx = glm::normalize(glm::cross(this->antidirection, absoluteUp));
        this->up = glm::normalize(glm::cross(positivx, this->antidirection));
    }

    public:

    float cameraSpeed{1.0f}, eoffset{30}, mouseSensitivity{0.3f};; //szybkość kamery WASD i 'rolli'

    float zoom{45.0f};

    enum movement
    {
        forwards, backwards, lefts, rights, ups, downs, pitchups, pitchdowns, yawups, yawdowns //todo roll
    };

    camera(glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec3 up = {0.0f, 1.0f, 0.0f}, float yaw = -90, float pitch = 0)
    {
        this->position = position;
        this->absoluteUp = up;
        this->yaw = yaw;
        this->pitch = pitch;

        updateCameraVectors();
    }

    glm::mat4 getViewMatrix()
    {
        //kamera, cel, góra
        return glm::lookAt(position, position + antidirection, up);
    }

    void moveWithKbd(movement dir, float deltaTime)
    {
        float velocity = cameraSpeed * deltaTime;

        switch (dir)
        {
            case forwards:
                position += antidirection * velocity;
                break;

            case backwards:
                position -= antidirection * velocity;
                break;

            case lefts:
                position -= positivx * velocity;
                break;

            case rights:
                position += positivx * velocity;
                break;
            
            case ups:
                position += up * velocity;
                break;

            case downs:
                position -= up * velocity;
                break;

            case pitchups:
                pitch += eoffset * velocity;
                break;

            case pitchdowns:
                pitch -= eoffset * velocity;
                break;

            case yawups:
                yaw += eoffset * velocity;
                break;

            case yawdowns:
                yaw -= eoffset * velocity;
                break;

            default:
                throw new recoverableExcept("Unknown camera move direction");
                break;
        }

        updateCameraVectors();
    }

    void moveWithMouse(float xoffset, float yoffset)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;
        updateCameraVectors();
    }

    void zoomWithMouse(float sc)
    {
        //XXXX TODO
        assert(1 == 0);
    }

};

}

#endif