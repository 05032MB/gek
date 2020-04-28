#ifndef GEK_IDRAWABLE_HPP
#define GEK_IDRAWABLE_HPP

#include <gek/misc.hpp>

#include <vector>

namespace GEK
{

struct iHasVertices
{
    protected:
    vbo vVertices;
    std::vector<float> vertices;
};
struct iHasColors
{
    protected:
    vbo vColors;
    std::vector<float> colors;
};
struct iHasNormals
{
    protected:
    vbo vNormals;
    std::vector<float> normals;
};
struct iHasTex
{
    protected:
    vbo vTex;
    std::vector<float> tex;
    bool usesTex;
};

struct iDrawable : public iHasVertices
{
    virtual void draw() = 0;
};

}

#endif