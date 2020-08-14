#ifndef GEK_OBJ_PRIMITIVE_HPP
#define GEK_OBJ_PRIMITIVE_HPP

#include <gek/primitivez/custom.hpp>
#include <gek/objModel.hpp>

namespace GEK
{

class objPrimitive : public customPrimitive
{
    public:

    objPrimitive(std::string name, std::string workingDir, float scale=1.0f)
    {
        loadObjFile(name, workingDir, &this->vertices, nullptr, nullptr, &this->tex);

        if(scale != 1.0f)
        {
            for(auto &i : vertices)
            {
                i *= scale;
            }
        }
    }

    void bind() override
    {
        this->primitive::bind(0, 3, 3, this->vertices, vVertices, false);
        this->primitive::bind(1, 2, 2, this->tex, vTex, false);
    }

    void draw() override
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 3); 
    }
};


}
#endif