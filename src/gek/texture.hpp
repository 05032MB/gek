#ifndef GEK_TEXTURE_HPP
#define GEK_TEXTURE_HPP

#include <gek/except.hpp>
#include <gek/interfacez/cullable.hpp>
#include <gek/misc.hpp>

#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace GEK
{

class texture : iCullable
{
    unsigned int texId;

    struct stbiDataDestroyer{
        void operator()(unsigned char * ptr){
            stbi_image_free(ptr);
        }
    }; 

    std::unique_ptr<unsigned char, stbiDataDestroyer> stbiData;
    int width, height, nChannels;

    std::string fileName;

    std::map<GLenum, GLenum> opts;

    public:

    //texOpts
    void setOpts(GLenum wrap, GLenum how = GL_MIRRORED_REPEAT, float *borderColor = nullptr )
    {
        glTexParameteri(GL_TEXTURE_2D, wrap, how);
        if(unlikely(how == GL_TEXTURE_BORDER_COLOR))
        {
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
        }
    }
    void saveOpts(GLenum dim, GLenum how)
    {
        opts[dim] = how;
    }
    //end

    texture(std::string where)
    {
        fileName = where;
        glGenTextures(1, &texId);
    }
    ~texture()
    {
        glDeleteTextures(1, &texId);
    }

    void bindTex()
    {
        glBindTexture(GL_TEXTURE_2D, texId);
    }

    void load()
    {
        stbiData.reset(stbi_load(fileName.c_str(), &width, &height, &nChannels, 0));

        if(!stbiData)
        {
            throw new failExcept("Could not load texture: " + fileName);
        }
    }

    void createTexture()
    {
        bindTex();
        for(const auto &i : opts)
        {
            setOpts(i.first, i.second);
        }

        load();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, stbiData.get());
        glGenerateMipmap(GL_TEXTURE_2D);

        stbiData.reset();
    }

    void cull() override
    {
        stbiData.reset();
        std::cout<<"Released tex "<<texId<<std::endl;
    }

};

}

#undef STB_IMAGE_IMPLEMENTATION

#endif