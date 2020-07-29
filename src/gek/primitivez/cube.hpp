#ifndef GEK_CUBE_HPP
#define GEK_CUBE_HPP

#include <gek/primitivez/primitive.hpp>

namespace GEK
{

class cube : public primitive
{
    public:

    cube()
    {
        this->vertices = std::vector{        
            //Ściana 1
            1.0f,-1.0f,-1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            -1.0f,-1.0f,-1.0f,1.0f,

            1.0f,-1.0f,-1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,

            //Ściana 2
            -1.0f,-1.0f, 1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,
            1.0f,-1.0f, 1.0f,1.0f,

            -1.0f,-1.0f, 1.0f,1.0f,
            -1.0f, 1.0f, 1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,


            //Ściana 3
            -1.0f,-1.0f,-1.0f,1.0f,
            1.0f,-1.0f, 1.0f,1.0f,
            1.0f,-1.0f,-1.0f,1.0f,

            -1.0f,-1.0f,-1.0f,1.0f,
            -1.0f,-1.0f, 1.0f,1.0f,
            1.0f,-1.0f, 1.0f,1.0f,

            //Ściana 4
            -1.0f, 1.0f, 1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,

            -1.0f, 1.0f, 1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,

            //Ściana 5
            -1.0f,-1.0f,-1.0f,1.0f,
            -1.0f, 1.0f, 1.0f,1.0f,
            -1.0f,-1.0f, 1.0f,1.0f,

            -1.0f,-1.0f,-1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            -1.0f, 1.0f, 1.0f,1.0f,

            //Ściana 6
            1.0f,-1.0f, 1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            1.0f,-1.0f,-1.0f,1.0f,

            1.0f,-1.0f, 1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
         };

         this->tex = std::vector{
            //Ściana 1
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

            //Ściana 2
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

            //Ściana 3
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

            //Ściana 4
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

            //Ściana 5
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

            //Ściana 6
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
         };
    }

    void bind() override
    {
        this->primitive::bind(0, 3, 4, this->vertices, vVertices);
        this->primitive::bind(1, 2, 2, this->tex, vTex);
    }

    void draw() override
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36); 
    }
};


}
#endif