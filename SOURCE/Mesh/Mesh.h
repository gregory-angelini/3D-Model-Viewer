#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/Shader.h>
#include <Texture/Texture2d.h>

class Mesh 
{
	public: struct Vertex 
	{
		glm::vec3 position; 
		glm::vec3 normal;  
		glm::vec2 texCoord; 
		glm::vec3 tangent; 
	};

	public: struct Texture 
	{
		unsigned int id;  
		Texture2d::Role type;  
	};

	private: std::vector<Vertex> vertices;  
	private: std::vector<std::shared_ptr<Texture2d>> textures;  
    private: std::vector<unsigned int> indices;  
	private: unsigned int VAO, VBO, EBO;      
    private: bool isInitialized = false;      

	public: Mesh();
	

	public: virtual ~Mesh();


	public: void Draw(std::shared_ptr<Shader> _shader);


	public: void Create(const std::vector<Vertex> &_vertices, const std::vector<unsigned int> &_indices, const std::vector<std::shared_ptr<Texture2d>> &_textures);


	public: void Destroy();

			
	public: bool IsExist()const;


	public: void AddTextureMap(std::shared_ptr<Texture2d> _texture);
};