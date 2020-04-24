#ifndef GEK_SHADER_PROGRAM_HPP
#define GEK_SHADER_PROGRAM_HPP

#include <gek/shader.hpp>
#include <gek/except.hpp>

#include <memory>
#include <iostream>

namespace GEK
{

class shaderProgram
{
    unsigned int shaderProgramId;

    std::vector<std::shared_ptr<shader>> shaders;

    void initShaderProgram()
    {
        shaderProgramId = glCreateProgram();
        if(shaderProgramId == 0)
        {
            throw new failExcept("Shader program init failed.");
        }
    }

    #define GLUE_UNIFORM_SIMPLE(postfix, name, t) glUniform##postfix(getUniform(name), t)
    #define GLUE_UNIFORM_VEC(postfix, name, t) glUniform##postfix##v(getUniform(name), 1, glm::value_ptr(t))
    #define GLUE_UNIFORM_MAT(postfix, name, t) glUniformMatrix##postfix##v(getUniform(name), 1, false, glm::value_ptr(t))

    #define GEN_SETUNIFORM(type, postfix, expected)            \
    void setUniform(std::string name, type t)                  \
    {                                                          \
        GLUE_UNIFORM_##expected(postfix, name, t);              \
    }

public:

    shaderProgram()
    {
        initShaderProgram();
    }

    ~shaderProgram()
    {
        if(shaderProgramId)
        {
            glDeleteProgram(shaderProgramId);
        }
    }

    void enslaveShader(std::shared_ptr<shader> sh)
    {
        shaders.push_back(sh);
    }

    template<typename... Args>
    void enslaveShader(Args && ... shr)
    {
        (shaders.push_back(shr), ...);
    }

    void compile()
    {
        for(const auto & i : shaders)
        {
            if(i->getId() == 0)
            {
                i->createShader();
            }

            glAttachShader(shaderProgramId, i->getId());
        }
        glLinkProgram(shaderProgramId);

        int suc;
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &suc);
        if(!suc)
        {
            constexpr auto size = 1024;
            std::aligned_storage<sizeof(char), alignof(char)> res[size];
            glGetProgramInfoLog(shaderProgramId, size, nullptr, reinterpret_cast<char *>(&res[0]));

            throw new failExcept("Could not link shader program: " + std::string(reinterpret_cast<char *>(&res[0])));
        }
    }

    void releaseShaders()
    {
        std::cout<<"Shader cull begin for program "<<shaderProgramId<<std::endl;
        for(auto & i : shaders)
        {
            i.reset();
        }
    }

    int getUniform(std::string name) const
    {
        int loc = glGetUniformLocation(shaderProgramId, name.c_str());
        if(loc == -1)
        {
            throw new failExcept("Shader program " + std::to_string(shaderProgramId) + " does not have uniform: " + name);
        }
        return loc;
    }

    //overloads of setUniform
    GEN_SETUNIFORM(float, 1f, SIMPLE);
    GEN_SETUNIFORM(double, 1d, SIMPLE);
    GEN_SETUNIFORM(int, 1i, SIMPLE);
    GEN_SETUNIFORM(unsigned int, 1ui, SIMPLE);

    GEN_SETUNIFORM(const glm::vec2&, 2f, VEC);
    GEN_SETUNIFORM(const glm::dvec2&, 2d, VEC);
    GEN_SETUNIFORM(const glm::ivec2&, 2i, VEC);

    GEN_SETUNIFORM(const glm::vec3&, 3f, VEC);
    GEN_SETUNIFORM(const glm::dvec3&, 3d, VEC);
    GEN_SETUNIFORM(const glm::ivec3&, 3i, VEC);

    GEN_SETUNIFORM(const glm::vec4&, 4f, VEC);
    GEN_SETUNIFORM(const glm::dvec4&, 4d, VEC);
    GEN_SETUNIFORM(const glm::ivec4&, 4i, VEC);

    GEN_SETUNIFORM(const glm::mat2&, 2f, MAT);
    GEN_SETUNIFORM(const glm::dmat2&, 2d, MAT);

    GEN_SETUNIFORM(const glm::mat3&, 3f, MAT);
    GEN_SETUNIFORM(const glm::dmat3&, 3d, MAT);

    GEN_SETUNIFORM(const glm::mat4&, 4f, MAT);
    GEN_SETUNIFORM(const glm::dmat4&, 4d, MAT);

    GEN_SETUNIFORM(const glm::mat2x3&, 2x3f, MAT);
    GEN_SETUNIFORM(const glm::dmat2x3&, 2x3d, MAT);

    GEN_SETUNIFORM(const glm::mat3x2&, 3x2f, MAT);
    GEN_SETUNIFORM(const glm::dmat3x2&, 3x2d, MAT);

    GEN_SETUNIFORM(const glm::mat2x4&, 2x4f, MAT);
    GEN_SETUNIFORM(const glm::dmat2x4&, 2x4d, MAT);

    GEN_SETUNIFORM(const glm::mat4x2&, 4x2f, MAT);
    GEN_SETUNIFORM(const glm::dmat4x2&, 4x2d, MAT);

    GEN_SETUNIFORM(const glm::mat3x4&, 3x4f, MAT);
    GEN_SETUNIFORM(const glm::dmat3x4&, 3x4d, MAT);

    GEN_SETUNIFORM(const glm::mat4x3&, 4x3f, MAT);
    GEN_SETUNIFORM(const glm::dmat4x3&, 4x3d, MAT);

    //end

    void activate() noexcept
    {
        glUseProgram(shaderProgramId);
    }

    static void deactivate() noexcept
    {
        glUseProgram(0);
    }

    unsigned int getId() const
    {
        return shaderProgramId;
    }

};

}
#endif