#pragma once
#include <Shader/Shader.h>



class Lamp
{
	public: class PointLight
	{
		public: glm::vec3 color;  
		public: glm::vec3 position;    

		public: glm::vec3 ambient;   
		public: glm::vec3 diffuse;   
		public: glm::vec3 specular;   

		public: float constant;   
		public: float linear;   
		public: float quadratic;   

		public: glm::mat4 model;  
		public: float radius;       
		public: float moveSpeed;    
		public: float t;   
	};

	private: GLuint VBO, VAO;   
	private: std::shared_ptr<Shader> shader;     
	public: PointLight state;   
	
	public: Lamp();


	public: virtual ~Lamp();


	private: void Destroy();


	private: void Init();


	private: void ShaderInit();


	private: void MeshInit();

			
	private: std::string GetVertexShaderSource();


	private: std::string GetFragmentShaderSource();


	public: void Draw(const glm::mat4 &_projectionView);


	public: void Draw(std::shared_ptr<Shader> _shader);


	public: void Update(float _delta);
};
