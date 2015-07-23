#include "manager/MeshManager.h"
#include "utils/Assert.h"
#include "utils/File.h"
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using ce::MeshManager;

ce::BorrowedBufferObject MeshManager::get( const std::string& file ) {
    auto it = resources_.find( file );
    
    if ( it != resources_.end() ) {
        return BorrowedBufferObject( it->second.get() );
    }
    
    #ifdef DEBUG
    ASSERT( ce::FileExists( file ), std::string("error: no such mesh file: " ) + file );
    #endif
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( 
        file.c_str(),
        aiProcess_Triangulate            | 
        aiProcess_GenSmoothNormals
    );
    
    #ifdef DEBUG
    ASSERT( scene, std::string("error: couldn't load model ") + file );
    #endif
    
    std::vector<float> data;
    
    for ( std::size_t i = 0u; i < scene->mNumMeshes; i++ ) {
        aiMesh* mesh = scene->mMeshes[i];
        int numFaces = mesh->mNumFaces;
        for ( int j = 0; j < numFaces; j++ ) {
            const aiFace& face = mesh->mFaces[j];
            for ( std::size_t k = 0u; k < 3u; k++ ) {
                
                aiVector3D vert = mesh->mVertices[ face.mIndices[k] ];
                data.push_back( vert.x ); data.push_back( vert.y ); data.push_back( vert.z );
                
                if ( mesh->HasNormals() ) {
                    aiVector3D norm = mesh->mNormals[ face.mIndices[k] ];
                    data.push_back( norm.x ); data.push_back( norm.y ); data.push_back( norm.z );
                }
            }
        }
    }
    resources_.emplace( file, std::make_unique<ce::BufferObject>( GL_ARRAY_BUFFER ) );
    ce::BufferObject* vbo = resources_.find( file )->second.get();
    vbo->dataStore(
        data.size(), sizeof(float), &data[0], GL_STATIC_DRAW
    );
    
    return BorrowedBufferObject( vbo );
}

void MeshManager::clear() {
    resources_.clear();
}

std::size_t MeshManager::size() const {
    return resources_.size();
}
