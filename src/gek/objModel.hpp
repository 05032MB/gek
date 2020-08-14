#ifndef GEK_OBJMODEL_HPP
#define GEK_OBJMODEL_HPP

#include <gek/except.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace GEK
{

/*
Adapted from example on tinyObjLoader site
*/
void loadObjFile(std::string inputfile, std::string materialDir, std::vector<float> *vertices, std::vector<float> *colors, std::vector<float> *normals, std::vector<float> *texcoords)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), !materialDir.empty() ? materialDir.c_str() : nullptr);

    if (!warn.empty()) {
        //throw new recoverableExcept(warn);
    }

    if (!err.empty()) {
        //throw new failExcept(err); //kto by się przejmował
    }

    if (!ret) {
        throw new failExcept("Model will not load");
    }

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                
                /*tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];*/
                if(vertices != nullptr)
                {
                    vertices->insert(vertices->end(), attrib.vertices.begin() + 3 * idx.vertex_index, attrib.vertices.begin() + 3 * idx.vertex_index + 3);
                }

                /*tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];*/
                if(normals != nullptr)
                {
                    normals->insert(normals->end(), attrib.normals.begin() + 3 * idx.normal_index, attrib.normals.begin() + 3 * idx.normal_index + 3);
                }

                /*tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
                tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];*/
                if(texcoords != nullptr)
                {
                    texcoords->insert(texcoords->end(), attrib.texcoords.begin() + 2 * idx.texcoord_index, attrib.texcoords.begin() + 2 * idx.texcoord_index + 2);
                }
                // Optional: vertex colors
                // tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
                // tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
                // tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
                if(colors != nullptr)
                {
                    colors->insert(colors->end(), attrib.colors.begin() + 3 * idx.vertex_index, attrib.colors.begin() + 3 * idx.vertex_index + 3);
                }
            }
                index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
}

}

#endif