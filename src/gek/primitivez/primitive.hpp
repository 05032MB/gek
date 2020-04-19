#ifndef GEK_PRIMITIVES_HPP
#define GEK_PRIMITIVES_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>


class primitive
{
    protected:
    unsigned int vbo, vao; //vertex buffer object, vertex array buffer

    std::vector<float> vertices;

    public:
    primitive()
    {
        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);
    }
    ~primitive()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void bind(int index, int size, int stride, bool normalize = false, GLenum usage = GL_STATIC_DRAW)
    {
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), usage);

        glVertexAttribPointer(index, size, GL_FLOAT, normalize, stride * sizeof(float), (void*)0);
        glEnableVertexAttribArray(index);  

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0); 
    }

    virtual void bind() = 0;

    virtual void draw() = 0;

};

#endif