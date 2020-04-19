#ifndef GEK_MISC_HPP
#define GEK_MISC_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include <gek/except.hpp>

namespace GEK
{
    void initGLFW()
    {
        if(glfwInit() == GLFW_FALSE)
        {
            throw new failExcept("GLFW init failed.");
        }
    }

    void initGLEW()
    {
        if (auto e = glewInit() != GLEW_OK) 
        {
            throw new failExcept("GLEW init failed: " + *glewGetErrorString(e));
        }
    }

}

#endif