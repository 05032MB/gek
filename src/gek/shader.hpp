#ifndef GEK_SHADER_HPP_
#define GEK_SHADER_HPP_

#include <GL/glew.h>
#include <string.h>

#include <fstream>
#include <vector>
#include <memory>

#include <gek/except.hpp>

namespace GEK
{

class shader
{
    unsigned int shaderId{0};

    std::string shaderFileName;
    std::unique_ptr<std::string> shaderBuffer;

    GLenum type; 

    public:
    
    shader(std::string filename, GLenum t)
    {
        shaderFileName = filename;
        type = t;
    }

    ~shader()
    {
        std::cout<<"Freeing shader "<<shaderId<<std::endl;
        glDeleteShader(shaderId);
    }

    void createShader()
    {
        std::cout<<"Preparing shader "<<shaderFileName<<std::endl;

        if(!shaderBuffer)
        {
            loadShader();
        }

        shaderId = glCreateShader(type);
        if(shaderId == 0)
        {
            throw new failExcept("Could not init shader: " + shaderFileName);
        }

        std::cout<<"Building shader "<<shaderFileName<<std::endl;

        auto * tmp = shaderBuffer->c_str();
        glShaderSource(shaderId, 1, &tmp, nullptr);
        glCompileShader(shaderId);

        int suc;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &suc);

        if(!suc)
        {
            constexpr auto size = 1024;
            std::aligned_storage<sizeof(char), alignof(char)> res[size];
            glGetShaderInfoLog(shaderId, size, nullptr, reinterpret_cast<char *>(&res[0]));

            throw new failExcept("Could not compile shader: " + std::string(reinterpret_cast<char *>(&res[0])));
        }

        shaderBuffer.reset();
    }

    void loadShader()
    {
        shaderBuffer = std::make_unique<std::string>();

        std::ifstream reader;

        errno = 0;

        reader.open(shaderFileName.c_str(), std::ios::in);

        if(reader.fail())
        {
            throw new failExcept("Cannot open shader file: " + shaderFileName + ", reason: " + strerror(errno) + ", bad? " + (reader.bad() ? "y" : "n"));
        }

        reader.seekg(0, std::ios::end);
        std::size_t size = reader.tellg();
        reader.seekg(0, std::ios::beg);

        std::cout<<"Loading shader "<<shaderFileName<<" of length "<<size<<std::endl;

        shaderBuffer->reserve(size);

        shaderBuffer->assign( (std::istreambuf_iterator<char>(reader) ),
                             (std::istreambuf_iterator<char>()     ) );

        reader.close();
    }

    auto getId() const noexcept
    {
        return shaderId;
    }
    auto getType() const noexcept
    {
        return type;
    }
    
};

}
#endif