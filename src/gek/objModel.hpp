#ifndef GEK_OBJMODEL_HPP
#define GEK_OBJMODEL_HPP

#include <gek/except.hpp>
#include <gek/misc.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace GEK
{
/*
Chwilowo jedyna funkcja to obliczanie wektorów normalnych, jeśli nie są podane w modelu.
*/
enum overrideObjLoad
{
    CalcNormalsFlat = 1 << 0,   //"Flat" - oblicza tylko z jednej powierzchni (face). TODO "Smooth" - uśrednione z wielu granicznych powierzchni
    NoColor     = 1 << 1,
};

#define CAN_LOAD_NORMALS !(overrideLoad & CalcNormalsFlat)

/*
Adapted from example on tinyObjLoader site
*/
void loadObjFile(std::string inputfile, std::string materialDir, 
                 std::vector<float> *vertices, std::vector<float> *colors, std::vector<float> *normals, std::vector<float> *texcoords,
                 unsigned int overrideLoad = 0
                )
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
                if(CAN_LOAD_NORMALS && normals != nullptr)
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
                if(!!(overrideLoad & NoColor) && normals != nullptr)    //No colors
                {
                    std::vector<float> t(3, 1.0f);
                    colors->insert(colors->end(), t.begin(), t.end());
                }
                else if(colors != nullptr)
                {
                    colors->insert(colors->end(), attrib.colors.begin() + 3 * idx.vertex_index, attrib.colors.begin() + 3 * idx.vertex_index + 3);
                }
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];

            /*
            Adapted from instructions here: https://support.microsoft.com/en-us/help/131130/how-to-set-the-current-normal-vector-in-an-opengl-application
            Niedokładne oszacowanie wektora normalnego - obliczane per face, i wrzucane do wierzchołków. Nie wygląda bardzo źle.
            Hańba temu kto nie dorzuca normalnych do swojego modelu.
            */
            if(!!(overrideLoad & CalcNormalsFlat) && normals != nullptr)
            {
                if(unlikely(fv != 3))
                {
                    throw new failExcept("Cannot calc flat normals for mesh with more than 3 vertices per shape.");
                }
                std::vector<float> extractor;
                for(auto i = 1; i <= 9; i++)
                {
                    extractor.push_back( vertices->at(vertices->size() - i) );
                }

                glm::vec3 a(extractor[0], extractor[1], extractor[2]);
                glm::vec3 b(extractor[3], extractor[4], extractor[5]);
                glm::vec3 c(extractor[6], extractor[7], extractor[8]);

                glm::vec3 planeNormal = glm::normalize(glm::cross(a - b, c - b));

                for(short i = 0; i < 3; i++)
                {
                    normals->push_back(planeNormal.x);
                    normals->push_back(planeNormal.y);
                    normals->push_back(planeNormal.z);
                }

            }

        }
    }
}

#undef CAN_LOAD_NORMALS

}

#endif