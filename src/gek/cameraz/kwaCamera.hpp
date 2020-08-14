#ifndef GEK_KWACAMERA_HPP
#define GEK_KWACAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gek/except.hpp>
#include <gek/interfacez/icamera.hpp>

namespace GEK
{

class kwaCamera : public iCameraStandardOps
{
    glm::vec3 position{0.0f, 0.0f, 0.0f}, absoluteUp{0, 1, 0}, right, left, front;
    glm::quat direction{0,0,0,-1}; //jednak kwaterniony

    float yawAng{0}, pitchAng{0}, rollAng{0};
    bool updUp;

    void updateCameraVectors()
    {
        //Yaw
        glm::quat aroundY = glm::angleAxis(glm::radians(-yawAng), glm::vec3(0, 1, 0));
        //Pitch
        glm::quat aroundX = glm::angleAxis(glm::radians(pitchAng), glm::vec3(1, 0, 0));
        //Roll - TODO
        //glm::quat aroundZ = glm::angleAxis(glm::radians(rollAng), glm::vec3(0, 0, 1));
        
        direction = aroundY * aroundX; //* aroundZ;
        
        glm::quat kwa = direction * glm::quat(0, 0, 0, -1) * glm::conjugate(direction);
        front = { kwa.x, kwa.y, kwa.z };

        if(updUp)
        {
            absoluteUp = glm::normalize(glm::cross(right, front));
            right = glm::normalize(glm::cross(front, absoluteUp));
            left = glm::normalize(glm::cross(absoluteUp, front));
        }
        else
        {
            right = glm::normalize(glm::cross(front, absoluteUp));
            left = glm::normalize(glm::cross(absoluteUp, front));
            absoluteUp = glm::normalize(glm::cross(right, front));
        }

        updUp = 0;
    }

    public:

    float cameraSpeed{1.0f}, eoffset{30}, mouseSensitivity{0.3f}; //szybkość kamery WASD i 'rolli'
    float zoom{45.0f};

    kwaCamera(glm::vec3 position = {0.0f, 0.0f, 0.0f})
    {
        this->position = position;
        this->direction = glm::quat(0,0,0,-1);

        glm::quat kwa = direction * glm::quat(0, 0, 0, -1) * glm::conjugate(direction);
        this->front = { kwa.x, kwa.y, kwa.z };
        this->right = glm::normalize(glm::cross(front, absoluteUp));
        this->left = glm::normalize(glm::cross(absoluteUp, front));

        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() override
    {
        glm::quat antidir = glm::conjugate(direction); //zrób kwa z kier patrzenia
        glm::mat4 rotate = glm::mat4_cast(antidir); // zrób z kwa obrót (kwa -> mat)

        glm::mat4 translate = glm::translate(glm::mat4(1.0), -position); //sprawdz przesunięcie od srodka

        return rotate * translate; //nałóż trans POTEM obroc !!!KOLEJNOŚĆ MA ZNACZENIE!!!
    }

    void moveWithKbd(movement dir, float deltaTime) override
    {
        float velocity = cameraSpeed * deltaTime;

        switch (dir)
        {
            case forwards:
                position += front * velocity;
                break;

            case backwards:
                position -= front * velocity;
                break;

            case lefts:
                position -= right * velocity;
                break;

            case rights:
                position += right * velocity;
                break;
            
            case ups:
                position += absoluteUp * velocity;
                break;

            case downs:
                position -= absoluteUp * velocity;
                break;

            case pitchups:
                pitchAng += eoffset * velocity;
                updUp = 1;
                break;

            case pitchdowns:
                pitchAng -= eoffset * velocity;
                updUp = 1;
                break;

            case yawups:
                yawAng -= eoffset * velocity;
                break;

            case yawdowns:
                yawAng += eoffset * velocity;
                break;

            case rollups:
                rollAng += eoffset * velocity;
                updUp = 1;
                break;

            case rolldowns:
                rollAng -= eoffset * velocity;
                updUp = 1;
                break;

            default:
                throw new recoverableExcept("Unknown camera move direction");
                break;
        }

        updateCameraVectors();
    }

    void moveWithMouse(float xoffset, float yoffset) override
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        if(xoffset)
        {
            updUp = 1;
            yawAng += xoffset;
            updateCameraVectors();
        }
        pitchAng += yoffset;
        updateCameraVectors();
    }

    void zoomWithMouse(float sc)
    {
        //XXXX TODO
        throw new failExcept("Zoom");
    }

};

}

#endif