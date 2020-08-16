#ifndef GEK_PRIMITIVES_HPP
#define GEK_PRIMITIVES_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <gek/interfacez/idrawable.hpp>

namespace GEK
{

class primitive : public iDrawable, public iHasTex, public iHasColors
{
    protected:
    unsigned int vao;

    template <typename T>
    using underlyingType = typename T::value_type;

    public:
    primitive()
    {
        glGenVertexArrays(1, &vao);
    }
    ~primitive()
    {
        glDeleteVertexArrays(1, &vao);
    }

    template <typename T>
    void bind(int index, int size, int stride, const T& data, unsigned int buff, bool normalize = false, GLenum usage = GL_STATIC_DRAW)
    {
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, buff);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(underlyingType<T>), data.data(), usage);

        glVertexAttribPointer(index, size, GL_FLOAT, normalize, stride * sizeof(underlyingType<T>), (void*)0);
        glEnableVertexAttribArray(index);  

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0); 
    }

    virtual void bind() = 0;

};

}
#endif