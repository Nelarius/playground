#include "manager/MeshManager.h"
#include "math/Vector.h"
#include "utils/Assert.h"
#include "utils/File.h"
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cmath>
#include <limits>

namespace pg {

opengl::BufferObject* MeshManager::get(const std::string& file) const {
    auto it = resources_.find(file);

    if (it != resources_.end()) {
        return it->second;
    }

    PG_ASSERT(pg::fileExists(file));

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        file.c_str(),
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals
        );

    PG_ASSERT(scene);

    float minx = std::numeric_limits< float >::max();
    float maxx = std::numeric_limits< float >::min();
    float miny = std::numeric_limits< float >::max();
    float maxy = std::numeric_limits< float >::min();
    float minz = std::numeric_limits< float >::max();
    float maxz = std::numeric_limits< float >::min();

    std::vector<float> data;

    for (std::size_t i = 0u; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        int numFaces = mesh->mNumFaces;
        for (int j = 0; j < numFaces; j++) {
            const aiFace& face = mesh->mFaces[j];
            for (std::size_t k = 0u; k < 3u; k++) {

                aiVector3D vert = mesh->mVertices[face.mIndices[k]];

                // find aabb bounding min/max extents
                if (vert.x < minx) {
                    minx = vert.x;
                }
                else if (vert.x > maxx) {
                    maxx = vert.x;
                }
                if (vert.y < miny) {
                    miny = vert.y;
                }
                else if (vert.y > maxy) {
                    maxy = vert.y;
                }
                if (vert.z < minz) {
                    minz = vert.z;
                }
                else if (vert.z > maxz) {
                    maxz = vert.z;
                }

                // stick the vertex into memory
                data.push_back(vert.x); data.push_back(vert.y); data.push_back(vert.z);

                // get the normal vector
                if (mesh->HasNormals()) {
                    aiVector3D norm = mesh->mNormals[face.mIndices[k]];
                    data.push_back(norm.x); data.push_back(norm.y); data.push_back(norm.z);
                }
            }
        }
    }
    auto index = buffer_.emplace<GLenum>(GL_ARRAY_BUFFER);
    resources_.emplace(file, &buffer_[index]);
    opengl::BufferObject* vbo = resources_.find(file)->second;
    vbo->dataStore(
        data.size(), sizeof(float), &data[0], GL_STATIC_DRAW
        );

    boxes_.emplace(
        file,
        math::AABoxf{
            math::Vec3f{ minx, miny, minz },
            math::Vec3f{ maxx, maxy, maxz }
    }
    );

    return vbo;
}

math::AABoxf MeshManager::getBoundingBox(const std::string& file) const {
    auto it = boxes_.find(file);
    PG_ASSERT(it != boxes_.end());
    return it->second;
}

void MeshManager::clear() {
    resources_.clear();
}

std::size_t MeshManager::size() const {
    return resources_.size();
}


}