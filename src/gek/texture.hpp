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
    bool flip{false};

    void bindTex(unsigned short numtex = 0)
    {
        if(numtex > 90)
        {
            throw new failExcept("Invalid texture number to bind, for texture: ", fileName);
        }
        glActiveTexture(GL_TEXTURE0 + numtex);
        glBindTexture(GL_TEXTURE_2D, texId);
    }

    public:

    enum texType {solo = 0, diffuse = 0, specular = 1, normal = 2, letDef = 99};

    texType defType{solo};

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

    texture(std::string where, bool flip = false, texType tt = solo)
    {
        fileName = where;
        this->flip = flip;
        this->defType = tt;
        glGenTextures(1, &texId);
    }
    texture(std::string where, texType tt)
    {
        fileName = where;
        this->defType = tt;
        glGenTextures(1, &texId);
    }
    ~texture()
    {
        glDeleteTextures(1, &texId);
    }

    void load()
    {
        stbi_set_flip_vertically_on_load(flip);
        stbiData.reset(stbi_load(fileName.c_str(), &width, &height, &nChannels, 0));

        if(!stbiData)
        {
            throw new failExcept("Could not load texture: " + fileName);
        }
    }

    void createTexture(unsigned texType = GL_RGB)
    {
        bindTex();
        for(const auto &i : opts)
        {
            setOpts(i.first, i.second);
        }

        load();

        glTexImage2D(GL_TEXTURE_2D, 0, texType, width, height, 0, texType, GL_UNSIGNED_BYTE, stbiData.get());
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbiData.reset();
    }

    void use(texType as = letDef)
    {
        bindTex(as == letDef ? defType : as);
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