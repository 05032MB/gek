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

         this->tex = std::vector{
            1.0f, 1.0f,
            1.0f, 0.0f, 
            0.0f, 1.0f,
            1.0f, 0.0f, 
            0.0f, 0.0f,
            0.0f, 1.0f
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
        glDrawArrays(GL_TRIANGLES, 0, 6); 
    }
};


}
#endif