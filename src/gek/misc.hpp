#ifndef GEK_MISC_HPP
#define GEK_MISC_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include <gek/except.hpp>

namespace GEK
{

#ifdef __GNUC__
#define unlikely(x) __builtin_expect(x, 0)
#define likely(x) __builtin_expect(x, 1)
#else
#warning "Profiling hinting impossible (don't worry it hardly matters)."
#define unlikely(x) x
#define likely(x) x
#endif

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

    struct vbo
    {
        unsigned int buffer;
        vbo()
        {
            glGenBuffers(1, &buffer);
        }
        ~vbo()
        {
            glDeleteBuffers(1, &buffer);
        }

        operator int() const
        {
            return buffer;
        }

    };

}

#endif