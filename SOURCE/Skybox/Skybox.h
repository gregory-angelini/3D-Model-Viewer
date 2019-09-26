#pragma once
#include <Shader/Shader.h>
#include <Texture/Texture2d.h>


class Skybox
{
	private: GLuint VBO, VAO;   
	private: std::shared_ptr<Texture2d> texture;  
	private: std::shared_ptr<Shader> shader;    
	private: glm::mat4 model;  


	public: Skybox();


	public: ~Skybox();


	private: void Init();


	private: void ShaderInit();


	private: void MeshInit();

			 
    private: void Destroy();


	public: std::shared_ptr<Texture2d> GetTexture()const;


	public: void Create(const std::vector<std::string> &_order);


	public: void SetScale(glm::vec3 _scale);


	public: void Draw(const glm::mat4 &_projectionView);
};

