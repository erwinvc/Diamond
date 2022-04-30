#pragma once
#include "graphics/buffers/vertexBufferLayout.h"
#include "util/path.h"
#include "assimp/postprocess.h"
#include "AssetLoader.h"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "math/math.h"

namespace Diamond {
	class Material;

	class ModelLoader : public AssetLoader {
	private:
		struct Vertex {
			glm::vec3 m_position;
			glm::vec3 m_normal;
			glm::vec2 m_uv;
			glm::vec3 m_tangents;
			glm::vec3 m_biTangents;
		};

		const unsigned int m_importFlags =
			aiProcess_RemoveRedundantMaterials |
			//aiProcess_GenUVCoords |
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_OptimizeMeshes |
			aiProcess_OptimizeGraph |
			aiProcess_Debone |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ImproveCacheLocality |
			aiProcess_FindDegenerates |
			aiProcess_FindInstances |
			aiProcess_FindInvalidData |
			aiProcess_ValidateDataStructure;

		struct MaterialData {
			Material* m_material;
			std::string m_roughness = "";
			std::string m_metallic = "";
			std::string m_albedo = "";
			std::string m_normal = "";
			std::string m_emission = "";

			MaterialData() {}
			MaterialData(Material* mat) : m_material(mat) {}
		};

		struct MeshData {
			std::string m_name;
			Vertex* m_vertices;
			std::vector<uint32_t> m_indices;
			glm::vec3 m_posMin = glm::vec3(Math::MAX_FLOAT, Math::MAX_FLOAT, Math::MAX_FLOAT);
			glm::vec3 m_posMax = glm::vec3(Math::MIN_FLOAT, Math::MIN_FLOAT, Math::MIN_FLOAT);
			int m_numVertices = 0;
			int m_materialIndex = 0;

			MeshData(const std::string& name, aiMesh* mesh, const aiScene* scene)
				: m_name(name), m_materialIndex(mesh->mMaterialIndex), m_numVertices(mesh->mNumVertices) {
				m_vertices = new Vertex[m_numVertices];
				bool hasTangents = mesh->HasTangentsAndBitangents();

				for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
					m_posMin.x = glm::min(m_posMin.x, mesh->mVertices[i].x);
					m_posMin.y = glm::min(m_posMin.y, mesh->mVertices[i].y);
					m_posMin.z = glm::min(m_posMin.z, mesh->mVertices[i].z);
					m_posMax.x = glm::max(m_posMax.x, mesh->mVertices[i].x);
					m_posMax.y = glm::max(m_posMax.y, mesh->mVertices[i].y);
					m_posMax.z = glm::max(m_posMax.z, mesh->mVertices[i].z);
					m_vertices[i].m_position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
					m_vertices[i].m_normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
					m_vertices[i].m_uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

					if (hasTangents) m_vertices[i].m_tangents = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
					if (hasTangents) m_vertices[i].m_biTangents = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
				}

				for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
					aiFace face = mesh->mFaces[i];
					for (uint32_t j = 0; j < face.mNumIndices; j++)
						m_indices.push_back(face.mIndices[j]);
				}
			}
			~MeshData();
		};

		Path m_path;
		//std::map<std::string, TextureLoader> m_textureData;
		std::vector<MeshData> m_meshData;
		std::vector<MaterialData> m_materialData;

		bool m_loadMaterials = false;
		int m_width = 0;
		int m_height = 0;
		void loadMaterials(const aiScene* scene) {

		}
		std::string loadTexture(int index, aiMaterial* mat, aiTextureType type) {

		}

	public:
		ModelLoader(const std::string& name, const Path& path, bool loadMaterials = true) : AssetLoader(name), m_loadMaterials(loadMaterials), m_path(path) {}

		void ProcessNode(aiNode* node, const aiScene* scene);

		void asyncLoad() override;
		void syncLoad() override;

		float getProgress() override;
	};
}