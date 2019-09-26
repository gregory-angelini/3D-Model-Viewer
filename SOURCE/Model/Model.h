#pragma once

#include <vector>
#include <Mesh/Mesh.h>
#include <Texture/Texture2d.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
    private: std::vector<std::shared_ptr<Texture2d>> uniqueTextures;   
	private: std::vector<std::shared_ptr<Mesh>> meshes;     
	private: bool isInitialized = false;     
    private: Texture2d::FilteringMode textureFilteringMode = Texture2d::FilteringMode::POINT_FILTERING;   

	public: Model();


	public: void SetTextureFilteringMode(Texture2d::FilteringMode _mode);


	public: void Draw(std::shared_ptr<Shader> _shader);


	public: std::vector<std::shared_ptr<Mesh>>& GetMeshes();


	public: void Create(const std::string &_path);


	private: void ProcessNode(aiNode* _node, const aiScene* _scene, const std::string &_directory);


	private: std::shared_ptr<Mesh> ProcessMesh(aiMesh* _mesh, const aiScene* _scene, const std::string &_directory);


	private: std::vector<std::shared_ptr<Texture2d>> LoadMaterialTextures(aiMaterial* _material, aiTextureType _type, Texture2d::Role _role, const std::string &_directory);

			 
	public: void Destroy();


	public: bool IsExist()const;


	public: bool AddTextureMap(std::shared_ptr<Texture2d> _texture,  int _index);


	public: bool AddTextureMap(std::string _path, Texture2d::Role _type, int _index);
};