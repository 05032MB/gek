#ifndef GEK_TRIANGLE_HPP
#define GEK_TRIANGLE_HPP

#include <gek/primitivez/primitive.hpp>

namespace GEK
{

class triangle : public primitive
{
    public:

    triangle()
    {
        this->vertices = std::vector{        
        -1.0f, -1.0f, 0.0f, // left  
         1.0f, -1.0f, 0.0f, // right 
         0.0f,  1.0f, 0.0f  // top   
         };
    }

    void bind() override
    {
        this->primitive::bind(0, 3, 3);
    }

    void draw() override
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3); 
    }
};

}
#endif