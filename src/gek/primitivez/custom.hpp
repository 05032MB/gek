#ifndef GEK_CUSTOM_HPP
#define GEK_CUSTOM_HPP

#include "primitive.hpp"

#include <gek/except.hpp>

namespace GEK
{

class customPrimitive : public primitive
{
    public:

    template<typename... Args>
    void replaceVertices(Args ... vcs)
    {
        vertices = {};
        (vertices.push_back(vcs), ...);
    }

    void bind() override
    {
        throw new failExcept("You need to manually specify binding parameters.");
    }

};

}

#endif