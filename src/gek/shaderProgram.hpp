#ifndef GEK_SHADER_PROGRAM_HPP
#define GEK_SHADER_PROGRAM_HPP

#include <gek/shader.hpp>
#include <gek/except.hpp>

#include <memory>
#include <iostream>
class shaderProgram
{
    unsigned int shaderProgramId;

    std::vector<std::unique_ptr<shader>> shaders;

    void initShaderProgram()
    {
        shaderProgramId = glCreateProgram();
        if(shaderProgramId == 0)
        {
            throw new failExcept("Shader program init failed.");
        }
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

    void enslaveShader(std::unique_ptr<shader> sh)
    {
        shaders.push_back(std::move(sh));
    }

    template<typename... Args>
    void enslaveShader(Args && ... shr)
    {
        (shaders.push_back(std::move(shr)), ...);
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

    void cullShaders()
    {
        std::cout<<"Shader cull begin for program "<<shaderProgramId<<std::endl;
        for(auto & i : shaders)
        {
            i.reset();
        }
    }

    void activate() noexcept
    {
        glUseProgram(shaderProgramId);
    }

    static void deactivate() noexcept
    {
        glUseProgram(0);
    }

};

#endif