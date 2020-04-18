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

    auto createWindow(int w, int h, std::string title)
    {
        auto *window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);

        if(window == nullptr)
        {
            throw new recoverableExcept("Window init failed");
        }

        return window;
    }

}

#endif