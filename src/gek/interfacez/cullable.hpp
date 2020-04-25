#ifndef GEK_CULLABLE_HPP
#define GEK_CULLABLE_HPP

namespace GEK
{

/*
Signifies that an object can be "culled" of currently unused resources (can free resources not actively in use)
The object should be able to function unhindered after the cull
*/
struct iCullable
{
    virtual void cull() = 0;
};

//Object can easily reverse the "culling" process and restore its previous state. Idk why, placeholder
struct iCullableReversible
{
    virtual void reverseCull() = 0;
}; 

}

#endif