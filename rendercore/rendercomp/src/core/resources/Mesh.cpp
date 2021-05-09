/*
 * This file is part of the RenderEngine distribution (https://github.com/NadirRoGue/RenderEngine).
 * Copyright (c) 2021 Nadir Román Guerrero.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <rendercomp/common/FilesystemUtils.h>
#include <rendercomp/common/Types.h>
#include <rendercomp/core/resources/Mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <set>

namespace rendercomp
{
namespace data
{

class SupportedExtensions
{
public:
    static bool isSupported(const String& path)
    {
        static SupportedExtensions extMng;
        const String ext = FilesystemUtils::getFileExtension(path);

        std::cout << "Extension: " << ext << std::endl;
        for(auto& supported : extMng._extension)
            std::cout << "\t" << supported << std::endl;


        return extMng._extension.find(ext) != extMng._extension.end();
    }
private:
    SupportedExtensions()
    {
        Assimp::Importer imp;
        std::string extensionsString;
        imp.GetExtensionList(extensionsString);

        std::istringstream stream(extensionsString);
        std::string s;
        while (std::getline(stream, s, ';'))
        {
           auto pos = s.find_last_of(".");
           if(pos != std::string::npos)
           {
               std::string ext = s.substr(pos + 1);
               std::transform(ext.begin(), ext.end(), ext.begin(), [](const char c)
               {
                   return tolower(c);
               });
               _extension.insert(String(ext.c_str()));
           }
        }
    }

private:
    std::set<String> _extension;
};

class MeshLoader
{
public:
    /**
     * @brief loadMesh Loads a mesh containing file into memory as a rendeng::Mesh object,
     *        stores it on an internal cache and returns a pointer to it.
     * @param path String with the path to the file
     * @param loadSettings Settings to tune the file laoading
     * @return A pointer to the cache-stored rendeng::Mesh
     * @throws std::runtime_error if any check fails though out the loading process
     */
    std::unique_ptr<Mesh> loadMesh(const String& path)
    {
        if(!FilesystemUtils::exists(path))
            throw std::runtime_error("The file " + TOSTDSTR(path) + " does not exists");

        // Set assimp import flags
        uint32_t loadFlags {0};

        loadFlags |= aiPostProcessSteps::aiProcess_JoinIdenticalVertices;
        loadFlags |= aiPostProcessSteps::aiProcess_SortByPType;
        loadFlags |= aiPostProcessSteps::aiProcess_Triangulate;

        // Read file from disk
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.c_str(), loadFlags);
        // Check everything loaded
        _checkImportedHealth(importer, scene, path);

        // Generate new mesh
        std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>();
        Mesh* dstMesh = newMesh.get();
        // Select mesh from assimp scene
        const size_t meshIndex = _selectFirstTriangleMesh(path, scene);
        const aiMesh* srcMesh = scene->mMeshes[meshIndex];

        // Load mesh data
        try
        {
            _readFaces(dstMesh, srcMesh);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Error reading faces from " + TOSTDSTR(path) + ":\n"
                                     + std::string(e.what()));
        }

        try
        {
            _readVertices(dstMesh, srcMesh);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Error reading vertex positions from " +TOSTDSTR(path)+ ":\n"
                                     + std::string(e.what()));
        }

        try
        {
            _readNormals(dstMesh, srcMesh);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Error reading vertex normals from " +TOSTDSTR(path)+ ":\n"
                                     + std::string(e.what()));
        }

        try
        {
            _readTangents(dstMesh, srcMesh);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Error reading vertex tangents from " +TOSTDSTR(path)+ ":\n"
                                     + std::string(e.what()));
        }

        try
        {
            _readUVs(dstMesh, srcMesh, path);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Error reading vertex uvs from " +TOSTDSTR(path)+ ":\n"
                                     + std::string(e.what()));
        }

        try
        {
            _readColors(dstMesh, srcMesh, path);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Error reading vertex colors from " +TOSTDSTR(path)+ ":\n"
                                     + std::string(e.what()));
        }

        // Release assimp resources
        importer.FreeScene();
        // Return pointer to newly loaded mesh
        return newMesh;
    }
private:
    /**
     * @brief _checkImportedHealth Check that the assimp load process completed successfully,
     *        and throws error otherwise
     * @param importer The Assimp::Importer object used to read the scene
     * @param scene The aiScene object resulting from the read
     * @param path The path to the file given to assimp for reading
     * @throws std::runtime_error if the read failed and the scene is null, or if the scene
     *         contains no meshes.
     */
    void _checkImportedHealth(const Assimp::Importer& importer,
                              const aiScene* scene,
                              const String& path)
    {
        // Check if read failed
        if(scene == nullptr)
            throw std::runtime_error("Error loading mesh file from " + TOSTDSTR(path) + ":\n"
                                     + std::string(importer.GetErrorString()));

        // Check if th
        if(scene->mNumMeshes < 1)
            throw std::runtime_error("The file " + std::string(path.c_str()) + " does not contain"
                                     " any mesh");

    }

    /**
     * @brief _selectFirstTriangleMesh searches the assimp scene to find a fully triangle
     *        composed mesh which will be the one loaded by the engine.
     * @param path String: The source file path being loaded, used here for debugging purposes
     * @param scene aiScene*: Pointer to the assimp scene which we will search for meshes
     * @return the index of the first fully triangle composed mesh in the scene
     * @throw std::runtime_error if no fully triangle composed meshes were found in the scene
     */
    size_t _selectFirstTriangleMesh(const String& path, const aiScene* scene)
    {
        // First find the first triangle mesh which we will load
        size_t triangleMeshIndex = 0;
        bool found = false;
        for(size_t i = 0; i < scene->mNumMeshes && !found; ++i)
        {
            if(scene->mMeshes[i]->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_TRIANGLE)
            {
                triangleMeshIndex = i;
                found = true;
            }
        }

        if(!found)
            throw std::runtime_error("Mesh loading: Could not find a triangle mesh in "
                                     + TOSTDSTR(path));

        // Check if other parts of the file were ignored for not being made out of triangles
        // Give just one warning and stop checking.
        for(size_t i = 0; i < scene->mNumMeshes; ++i)
        {
            if(scene->mMeshes[i]->mPrimitiveTypes != aiPrimitiveType::aiPrimitiveType_TRIANGLE)
            {
                std::cout << "Mesh loading: Some parts of " << path.c_str() << " were ignored as "
                          << "they are not formed by triangle primitives";
                break;
            }
        }

        return triangleMeshIndex;
    }

    /**
     * @brief _readFaces Read the faces from the aiMesh into the rendeng::Mesh object.
     *        If the mesh was loaded using flat shading, the faces are generated automatically
     *        rather than read from the file, as they will not match the underlying morphology.
     * @param dst rendeng::Mesh* object where to store the faces
     * @param src aiMesh* object from where to read the faces
     * @param flatShading flag indicating wether the mesh was loaded with flat shading
     * @param path The path from where the mesh is being loaded, used for debugging purposes
     * @throws std::runtime_error if the mesh was loaded with flat shading and the number of
     *         vertices % 3 is not equal to 0, or if the number of faces is not equal to the
     *         number of vertices divided by 3.
     */
    void _readFaces(Mesh* dst, const aiMesh* src)
    {
        if(!src->HasFaces())
            throw std::runtime_error("The mesh has no index data");

        dst->faces.resize(src->mNumFaces, {0, 0, 0});
        for(size_t i = 0; i < src->mNumFaces; ++i)
        {
            if(src->mFaces[i].mNumIndices != 3)
                std::cout << src->mFaces[i].mNumIndices << std::endl;
            dst->faces[i].x = src->mFaces[i].mIndices[0];
            dst->faces[i].y = src->mFaces[i].mIndices[1];
            dst->faces[i].z = src->mFaces[i].mIndices[2];
        }
    }

    /**
     * @brief _readVertices Read vertex positions from the assimp mesh into the rendeng::Mesh
     *        object.
     * @param dst rendeng::Mesh object where the vertex positions will be stored
     * @param src aiMesh* object from where the vertex positions will be extracted
     */
    void _readVertices(Mesh* dst, const aiMesh* src)
    {
        if(!src->HasPositions())
            throw std::runtime_error("The mesh has no position data");

        dst->vertexPositions.resize(src->mNumVertices, {0.f, 0.f, 0.f});
        for(size_t i = 0; i < src->mNumVertices; ++i)
        {
            dst->vertexPositions[i].x = src->mVertices[i].x;
            dst->vertexPositions[i].y = src->mVertices[i].y;
            dst->vertexPositions[i].z = src->mVertices[i].z;
        }
    }

    /**
     * @brief _readNormals Read vertex normal vectors from the assimp mesh into the rendeng::Mesh
     *        object
     * @param dst rendeng::Mesh object where the vertex positions will be stored
     * @param src aiMesh* object from where the vertex positions will be extracted
     * @throws std::runtime_error if the normal vectors were not found in the source mesh
     */
    void _readNormals(Mesh* dst, const aiMesh* src)
    {
        if(!src->HasNormals())
            return;

        dst->vertexNormals.resize(src->mNumVertices, {0.f, 0.f, 0.f});
        for(size_t i = 0; i < src->mNumVertices; ++i)
        {
            dst->vertexNormals[i].x = src->mNormals[i].x;
            dst->vertexNormals[i].y = src->mNormals[i].y;
            dst->vertexNormals[i].z = src->mNormals[i].z;
        }
    }

    /**
     * @brief _readTangents Read vertex tangent vectors from the assimp mesh into the rendeng::Mesh
     *        object, if available
     * @param dst rendeng::Mesh object where tangent vectors will be stored
     * @param src aiMesh object from where tangent vectors will be extracted
     * @param tangentsLoaded flag indicating wether the mesh was loading requesting tangetn vector
     *        generation
     * @throws std::runtime_error if the tangents were requested by they are not present
     */
    void _readTangents(Mesh* dst, const aiMesh* src)
    {
        if(!src->HasTangentsAndBitangents())
            return;

        dst->vertexTangents.resize(src->mNumVertices, {0.f, 0.f, 0.f});
        for(size_t i = 0; i < src->mNumVertices; ++i)
        {
            dst->vertexTangents[i].x = src->mTangents[i].x;
            dst->vertexTangents[i].y = src->mTangents[i].y;
            dst->vertexTangents[i].z = src->mTangents[i].z;
        }
    }

    /**
     * @brief _readUVs Read all vertex uv map coordinate channels from the assimp mesh into
     *        the rendeng::Mesh object, if available
     * @param dst rendeng::Mesh object where to store the uv map channels
     * @param src aiMesh object from where to read the uv map channels
     * @param path String containing the mesh source file path, used for debugging purposes
     */
    void _readUVs(Mesh* dst, const aiMesh* src, const String& path)
    {
        const size_t numUVChannels = src->GetNumUVChannels();
        if(numUVChannels > 0)
        {
            dst->vertexUVs.resize(numUVChannels);
            for(size_t c = 0; c < numUVChannels; ++c)
            {
                dst->vertexUVs[c].resize(src->mNumVertices, {0.f, 0.f});
                if(src->HasTextureCoords(c))
                {
                    for(size_t i = 0; i < src->mNumVertices; ++i)
                    {
                        dst->vertexUVs[c][i].x = src->mTextureCoords[c][i].x;
                        dst->vertexUVs[c][i].y = src->mTextureCoords[c][i].y;
                    }
                }
                // If there are uv mapping channels, warn of any empty one
                else
                    std::cout << "Mesh loading: " << path.c_str()
                              << " misses uv mapping channel index " << c << std::endl;
            }
        }
    }

    /**
     * @brief _readColors Read all vertex color channels from the assimp mesh into
     *        the rendeng::Mesh object, if available
     * @param dst rendeng::Mesh object where to store the color channels
     * @param src aiMesh object from where to read the color channels
     * @param path String containing the mesh source file path, used for debugging purposes
     */
    void _readColors(Mesh* dst, const aiMesh* src, const String& path)
    {
        const size_t numColorChannels = src->GetNumColorChannels();
        if(numColorChannels > 0)
        {
            dst->vertexColors.resize(numColorChannels);
            for(size_t c = 0; c < numColorChannels; ++c)
            {
                dst->vertexColors[c].resize(src->mNumVertices, {0.f, 0.f, 0.f, 0.f});
                if(src->HasVertexColors(c))
                {
                    for(size_t i = 0; i < src->mNumVertices; ++i)
                    {
                        dst->vertexColors[c][i].r = src->mColors[c][i].r;
                        dst->vertexColors[c][i].g = src->mColors[c][i].g;
                        dst->vertexColors[c][i].b = src->mColors[c][i].b;
                        dst->vertexColors[c][i].a = src->mColors[c][i].a;
                    }
                }
                // If there are color channels, warn of any empty one
                else
                    std::cout << "Mesh loading: " << path.c_str()
                              << " misses color channel index " << c << std::endl;
            }
        }
    }
};

}

template<>
std::unique_ptr<data::Mesh>
Resources::get<data::Mesh>(const String& path)
{
    static data::MeshLoader loader;
    return loader.loadMesh(path);
}
}
