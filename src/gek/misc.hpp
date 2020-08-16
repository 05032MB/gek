#ifndef GEK_MISC_HPP
#define GEK_MISC_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

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

    std::vector<float> approxNormals(const std::vector<float> &vertices, unsigned stride)
    {
        int off = vertices.size();
        std::vector<float> normals;

        while(off >= 9)
        {
            std::vector<float> extractor;
            for(auto i = 1; i <= 9; i++)
            {
                extractor.push_back( vertices.at(off - i) );
            }

            glm::vec3 a(extractor[0], extractor[1], extractor[2]);
            glm::vec3 b(extractor[3], extractor[4], extractor[5]);
            glm::vec3 c(extractor[6], extractor[7], extractor[8]);

            glm::vec3 planeNormal = glm::normalize(glm::cross(a - b, c - b));

            for(short i = 0; i < 3; i++)
            {
                normals.push_back(planeNormal.x);
                normals.push_back(planeNormal.y);
                normals.push_back(planeNormal.z);
            }
            off -= stride;
        }
        return normals;
    }

}

#endif