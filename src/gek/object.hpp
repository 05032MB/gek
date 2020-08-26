#ifndef GEK_OBJECT_HPP
#define GEK_OBJECT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include <gek/primitivez/primitives.hpp>
#include <gek/texture.hpp>
#include <gek/except.hpp>

namespace GEK
{

class object : public iDrawable
{
    glm::vec3 pos;
    std::shared_ptr<primitive> model;
    std::vector<std::shared_ptr<texture>> texs;

    float yawAng{0}, pitchAng{0}, rollAng{0};

    bool needsToRecalcTransform{true};

    glm::mat4 cachedModel;

    glm::mat4 applyRotations()
    {
        glm::mat4 tmp(1.0f);
        tmp = glm::rotate(tmp, glm::radians(yawAng), glm::vec3(0, 1, 0));
        tmp = glm::rotate(tmp, glm::radians(pitchAng), glm::vec3(1, 0, 0));
        tmp = glm::rotate(tmp, glm::radians(rollAng), glm::vec3(0, 0, 1));
        return tmp;
    }

    public:
    glm::mat4 getModelMatrix()
    {
        return needsToRecalcTransform ? glm::translate(applyRotations(), pos) : cachedModel;
    }

    void draw() override
    {

        if(!model)
        {
            throw new recoverableExcept("No model set for object");
        }

        for(auto & tex : texs)
        {
            tex->use();
        }

        model->draw();
    }

    template<typename... Args>
    void enslaveTex(Args && ... xx)
    {
        (texs.push_back(xx), ...);
    }

    void enslaveModel(std::shared_ptr<primitive> model)
    {
        this->model = std::move(model);
    }

    void setPosition(glm::vec3 pos)
    {
        this->pos = pos;
        needsToRecalcTransform = true;
    }
    glm::vec3 getPosition() const
    {
        return this->pos;
    }

    enum whichAngle {pitch, yaw, roll};

    void setRotationAngle(whichAngle ang, float degrees)
    {
        switch (ang)
        {
            case whichAngle::pitch:
                this->pitchAng = degrees;
                break;
                
            case whichAngle::yaw:
                this->yawAng = degrees;
                break;

            case whichAngle::roll:
                this->rollAng = degrees;
                break;

            default:
                throw new recoverableExcept("Unknown angle to rotate object by");
                break;
        }
        needsToRecalcTransform = true;
    }
    float getRotationAngle(whichAngle ang, float degrees)
    {
        switch (ang)
        {
            case whichAngle::pitch:
                return this->pitchAng;
                
            case whichAngle::yaw:
                return this->yawAng;

            case whichAngle::roll:
                return this->rollAng;

            default:
                throw new recoverableExcept("Get unknown object angle");
                return 0;
        }
    }

};

}

#endif