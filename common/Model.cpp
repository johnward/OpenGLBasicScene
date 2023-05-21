#include "Model.h"

Model::Model()
{
}

void Model::LoadModel(const std::string &filename)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene)
    {
        printf("Model (%s) failed to load %s", filename.c_str(), importer.GetErrorString());
        return;
    }

    LoadNode(scene->mRootNode, scene);

    LoadMaterials(scene);
}

void Model::RenderModel()
{
    for (size_t i = 0; i < meshList.size(); i++)
    {
        unsigned int materialIndex = meshToTex[i];
        if (materialIndex < textureList.size() && textureList[materialIndex])
        {
            textureList[materialIndex]->UseTexture();
        }

        meshList[i]->RenderMesh();
    }
}

void Model::ClearModel()
{
    for (size_t i = 0; i < meshList.size(); i++)
    {
        if (meshList[i])
        {
            delete meshList[i];
            meshList[i] = nullptr;
        }
    }

    for (size_t i = 0; i < textureList.size(); i++)
    {
        if (textureList[i])
        {
            delete textureList[i];
            textureList[i] = nullptr;
        }
    }
}

Model::~Model()
{
}

void Model::LoadNode(aiNode *node, const aiScene *scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        LoadNode(node->mChildren[i], scene);
    }
}

void Model::LoadMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        GLfloat arr[] =  {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        vertices.insert(vertices.end(), arr, arr + 3);
        if (mesh->mTextureCoords[0])
        {
            GLfloat arr1[] = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            vertices.insert(vertices.end(), arr1, arr1+2);
        }
        else
        {
            GLfloat arr1[] =  {0.0f, 0.0f};
            vertices.insert(vertices.end(), arr1, arr1 + 2);
        }

        GLfloat arr1[] = {-mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z};
        vertices.insert(vertices.end(), arr1, arr1 + 3);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh *newMesh = new Mesh();
    newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
    meshList.push_back(newMesh);
    meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene *scene)
{
    textureList.resize(scene->mNumMaterials);

    for (size_t i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial *material = scene->mMaterials[i];
        textureList[i] = nullptr; //C++11 concept ???
        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                // sometimes texture have absolute path
                int idx = std::string(path.data).rfind("\\");
                std::string filename = std::string(path.data).substr(idx + 1);
                std::string texPath = std::string("textures/") + filename;
                textureList[i] = new Texture(texPath.c_str());

                if (!textureList[i]->LoadTexture())
                {
                    printf("Failed to load texture at: %s\n", texPath.c_str());
                    delete textureList[i];
                    textureList[i] = nullptr;
                }
            }
        }

        if (!textureList[i])
        {
            textureList[i] = new Texture("textures/plain.png");
            textureList[i]->LoadTexture();
        }
    }
}