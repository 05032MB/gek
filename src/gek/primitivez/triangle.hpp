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

        this->tex = std::vector{
         0.0f, 0.0f,  // lower-left corner  
         1.0f, 0.0f,  // lower-right corner
         0.5f, 1.0f   // top-center corner
        };

        this->colors = std::vector((this->vertices.size() / 4) * 3, 1.0f);

    }

    void bind() override
    {
        this->primitive::bind(0, 3, 3, this->vertices, vVertices);
        this->primitive::bind(1, 2, 2, this->tex, vTex);
        this->primitive::bind(3, 3, 3, this->colors, vColors);
    }

    void draw() override
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3); 
    }
};

}
#endif