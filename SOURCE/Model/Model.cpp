#include <Model/Model.h>
#include <File/File.h>


Model::Model()
{}


void Model::Destroy()
{
	if (IsExist())
	{
		meshes.clear();
		isInitialized = false;
	}
}

void Model::SetTextureFilteringMode(Texture2d::FilteringMode _mode)
{
	if (textureFilteringMode != _mode)
	{
		textureFilteringMode = _mode;

		for (auto it : uniqueTextures)
		{
			it->SetFilteringMode(textureFilteringMode);   
		}
	}
}


bool Model::IsExist()const
{
	return isInitialized;
}


void Model::Create(const std::string &_path)
{
	Assimp::Importer importer;  
	const aiScene *scene = importer.ReadFile(_path.c_str(),      
		                                     aiProcess_Triangulate |           
		                                     aiProcess_FlipUVs |       
		                                     aiProcess_CalcTangentSpace);      
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)     
	{
		std::cout << "Failed to load model: " << _path << ".Error code: " << importer.GetErrorString() << std::endl;
		return;
	}

	std::string directory = _path.substr(0, _path.find_last_of('/'));        
	ProcessNode(scene->mRootNode, scene, directory);   
	isInitialized = true;
}


void Model::ProcessNode(aiNode* _node, const aiScene* _scene, const std::string &_directory)
{
	for (unsigned int i = 0; i < _node->mNumMeshes; i++)
	{
		aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, _scene, _directory));     
	}
	for (unsigned int i = 0; i < _node->mNumChildren; i++)
	{
		ProcessNode(_node->mChildren[i], _scene, _directory);
	}
}


std::vector<std::shared_ptr<Mesh>>& Model::GetMeshes()
{
	return meshes;
}

void Model::Draw(std::shared_ptr<Shader> _shader)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw(_shader);
	}
}


std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* _mesh, const aiScene* _scene, const std::string &_directory)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture2d>> textures;
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(); 

	for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex;

		vertex.position.x = _mesh->mVertices[i].x;
		vertex.position.y = _mesh->mVertices[i].y;
		vertex.position.z = _mesh->mVertices[i].z;

		vertex.normal.x = _mesh->mNormals[i].x;
		vertex.normal.y = _mesh->mNormals[i].y;
		vertex.normal.z = _mesh->mNormals[i].z;

		if (_mesh->mTextureCoords[0])     
		{
			vertex.texCoord.x = _mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = _mesh->mTextureCoords[0][i].y;
		}
		else     
		{
			vertex.texCoord = glm::vec2(0.0f, 0.0f);
		}

		vertex.tangent.x = _mesh->mTangents[i].x;
		vertex.tangent.y = _mesh->mTangents[i].y;
		vertex.tangent.z = _mesh->mTangents[i].z;

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
	{
		aiFace face = _mesh->mFaces[i];   

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);   
		}
	}
		
	if (_mesh->mMaterialIndex >= 0)    
	{
		aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];  

		std::vector<std::shared_ptr<Texture2d>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, Texture2d::Role::DIFFUSE, _directory);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		std::vector<std::shared_ptr<Texture2d>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, Texture2d::Role::SPECULAR, _directory);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<std::shared_ptr<Texture2d>> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, Texture2d::Role::NORMAL, _directory);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}
	
	mesh->Create(vertices, indices, textures);
	return mesh;
}

bool Model::AddTextureMap(std::shared_ptr<Texture2d> _texture, int _index)
{
	if (_index >= 0 && _index < meshes.size())
	{
		if (_texture)
		{
			std::string fileName = _texture->GetName();   

			for (auto it : uniqueTextures)
			{
				if (it->GetName() == fileName)       
				{
					meshes[_index]->AddTextureMap(it);      
					return false;
				}
			}

			uniqueTextures.push_back(_texture);     

			meshes[_index]->AddTextureMap(_texture);      
			return true;
		}
	}
	return false;
}

bool Model::AddTextureMap(std::string _path, Texture2d::Role _role, int _index)
{
	if (_index >= 0 && _index < meshes.size())
	{
		std::string fileName = File::_GetBaseName(_path);   

		for (auto it: uniqueTextures)
		{
			if (it->GetName() == fileName)       
			{
				meshes[_index]->AddTextureMap(it);      
				return false;
			}
		}

		std::shared_ptr<Texture2d> texture = std::make_shared<Texture2d>();
		texture->SetFilteringMode(textureFilteringMode);   

		if (texture->Create(_path))   
		{
			texture->SetRole(_role);   
			texture->SetName(fileName);   

			uniqueTextures.push_back(texture);     
			
			meshes[_index]->AddTextureMap(texture);      
		}
		else
		{
			std::cout << "Failed to load texture for model: " << _path << std::endl;
		}
		return true;
	}
	return false;
}

std::vector<std::shared_ptr<Texture2d>> Model::LoadMaterialTextures(aiMaterial* _material, aiTextureType _type, Texture2d::Role _role, const std::string &_directory)
{
	std::vector<std::shared_ptr<Texture2d>> textures;

	unsigned int size = _material->GetTextureCount(_type);    

	for (unsigned int i = 0; i < size; i++)
	{
		aiString fileName;  
		_material->GetTexture(_type, i, &fileName);   

		bool skip = false;              

		for (unsigned int j = 0; j < uniqueTextures.size(); j++)
		{
			if (std::strcmp(uniqueTextures[j]->GetName().c_str(), fileName.C_Str()) == 0)
			{
				textures.push_back(uniqueTextures[j]);
				skip = true;
			}
		}

		if (!skip)        
		{
			std::shared_ptr<Texture2d> texture = std::make_shared<Texture2d>();
			texture->SetFilteringMode(textureFilteringMode);   

			std::string path = _directory + '/' + fileName.C_Str();      
	
			if (texture->Create(path))   
			{
				texture->SetRole(_role);   
				texture->SetName(File::_GetBaseName(fileName.C_Str()));   

				uniqueTextures.push_back(texture);     
				textures.push_back(texture);
			}
			else
			{
				std::cout << "Failed to load texture for model: " << path << std::endl;
			}
		}
	}
	return textures;
}
