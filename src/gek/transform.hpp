#ifndef GEK_TRANSFORM_HPP
#define GEK_TRANSFORM_HPP

#include <glm/glm.hpp>

#include <map>
#include <string>

#include <gek/shaderProgram.hpp>

namespace GEK{

template <typename T>
class transformObject
{
    protected:
    std::shared_ptr<shaderProgram> program;

    public:
    transformObject(std::shared_ptr<shaderProgram> sh)
    {
        enslaveProgram(sh);
    }
    void enslaveProgram(std::shared_ptr<shaderProgram> sh)
    {
        program = sh;
    }

    virtual void applyTransform(T arg) = 0;
    virtual void operator()(T arg) = 0;
};

class move : public transformObject<glm::vec3>
{
    public:
    using transformObject::transformObject;

    void applyTransform(glm::vec3 arg) override
    {
        program->setUniform("off", arg);
    }
    void operator() (glm::vec3 arg) override
    {
        applyTransform(arg);
    }
};

class scale : public transformObject<float>
{
    public:
    using transformObject::transformObject;

    void applyTransform(float arg) override
    {
        program->setUniform("scale", arg);
    }
    void operator() (float arg) override
    {
        applyTransform(arg);
    }
};

class transform : public transformObject<glm::mat4>
{
    public:
    using transformObject::transformObject;

    void applyTransform(glm::mat4 arg) override
    {
        program->setUniform("transform", arg);
    }
    void operator() (glm::mat4 arg) override
    {
        applyTransform(arg);
    }
};

class color : public transformObject<glm::vec4>
{
    public:
    using transformObject::transformObject;

    void applyTransform(glm::vec4 arg) override
    {
        program->setUniform("color", arg);
    }
    void operator() (glm::vec4 arg) override
    {
        applyTransform(arg);
    }
};

}
#endif