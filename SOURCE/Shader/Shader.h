#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Shader
{
	private: GLuint shaderProgramId = 0;   
    private: bool isInitialized = false;     

	public: Shader();
	

	public: GLuint GetId()const;


	public: void Bind();

			
	public: bool IsExist()const;


	private: bool CreateVertexShader(const GLchar* _source, GLuint& _id);


	private: bool CreateFragmentShader(const GLchar* _source, GLuint& _id);


	public: bool CreateShaderProgramFromSource(const std::string &_vertexShaderSource, const std::string &_fragmentShaderSource);


	public: bool CreateShaderProgramFromFile(const std::string &_vertexShaderPath, const std::string &_fragmentShaderPath);

			
	public: void SetValue(const std::string &_name, const glm::vec3 &_value);


	public: void SetValue(const std::string &_name, const glm::mat3 &_value);


	public: void SetValue(const std::string &_name, const glm::mat4 &_value);


	public: void SetValue(const std::string &_name, float _value);

			
	public: void SetValue(const std::string &_name, int _value);

			
	public: void SetValue(const std::string &_name, bool _value);
};
