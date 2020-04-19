#ifndef GEK_RECTANGLE_HPP
#define GEK_RECTANGLE_HPP

#include <gek/primitivez/primitive.hpp>

namespace GEK
{

class rectangle : public primitive
{
    public:

    rectangle()
    {
        this->vertices = std::vector{        
            1.0f,  1.0f, 0.0f,  // top right
            1.0f, -1.0f, 0.0f,  // bottom right
            -1.0f,  1.0f, 0.0f,  // top left 
            // second triangle
            1.0f, -1.0f, 0.0f,  // bottom right
            -1.0f, -1.0f, 0.0f,  // bottom left
            -1.0f,  1.0f, 0.0f   // top left
         };
    }

    void bind() override
    {
        this->primitive::bind(0, 3, 3);
    }

    void draw() override
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6); 
    }
};


}
#endif