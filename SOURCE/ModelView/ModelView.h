#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Math/glm.h>


class ModelView
{
	private: glm::vec3 position = glm::vec3(0.0f);    
	private: glm::vec3 scale = glm::vec3(0.0f);  
	private: glm::quat rotation = glm::init<glm::quat>::identity();    
	private: glm::mat4 transformation = glm::mat4(1.0f);   
	private: glm::mat3 normalMatrix = glm::mat3(1.0f);         
	private: bool transformationValid = false;        


	public: ModelView();
		
	public: void SetPosition(glm::vec3 _position);


	public: glm::vec3 GetPosition()const;


	public: void SetRotation(glm::quat _rotation);


	public: glm::quat GetRotation()const;


	public: void SetScale(glm::vec3 _scale);


	public: glm::vec3 GetScale()const;

			
	public: glm::mat3 GetNormalMatrix()const;

	public: glm::mat4 GetTransformation();


	public: void SetTransformation(glm::vec3 _scale, glm::quat _rotation, glm::vec3 _position);
};