#include "Shader.h"
#include <File/File.h>
#include <GL/glew.h>



Shader::Shader()
{}


void Shader::Bind()
{
	if (isInitialized)
	{
		glUseProgram(shaderProgramId);       
	}

}



bool Shader::IsExist()const
{
	return isInitialized;
}


GLuint Shader::GetId()const
{
	return shaderProgramId;
}


void Shader::SetValue(const std::string &_name, const glm::vec3 &_value)
{
	glUniform3f(glGetUniformLocation(shaderProgramId, _name.c_str()), _value.x, _value.y, _value.z);
}


void Shader::SetValue(const std::string &_name, int _value)
{
	glUniform1i(glGetUniformLocation(shaderProgramId, _name.c_str()), _value);
}


void Shader::SetValue(const std::string &_name, bool _value)
{
	glUniform1i(glGetUniformLocation(shaderProgramId, _name.c_str()), (int)_value);
}


void Shader::SetValue(const std::string &_name, float _value)
{
	glUniform1f(glGetUniformLocation(shaderProgramId, _name.c_str()), _value);
}


void Shader::SetValue(const std::string &_name, const glm::mat4 &_value)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, _name.c_str()),    
		               1,    
	             	   GL_FALSE,              
		               glm::value_ptr(_value));
}


void Shader::SetValue(const std::string &_name, const glm::mat3 &_value)
{
	glUniformMatrix3fv(glGetUniformLocation(shaderProgramId, _name.c_str()),    
		1,    
		GL_FALSE,              
		glm::value_ptr(_value));
}



bool Shader::CreateVertexShader(const GLchar* _source, GLuint& _id)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);   

	glShaderSource(vertexShader,     
		           1,    
	               &_source,    
		           NULL);

	glCompileShader(vertexShader);   

	GLint success;  

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);    

	if (!success)      
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);    
		std::cout << "Failed to compile vertex shader. Error code: " << infoLog << std::endl;
		return false;
	}

	_id = vertexShader;
	return true;
}



bool Shader::CreateFragmentShader(const GLchar* _source, GLuint& _id)
{
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);   

	glShaderSource(fragmentShader,     
		           1,    
		           &_source,    
		           NULL);

	glCompileShader(fragmentShader);   

	GLint success;  

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);    

	if (!success)      
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);    
		std::cout << "Failed to compile fragment shader. Error code: " << infoLog << std::endl;
		return false;
	}

	_id = fragmentShader;
	return true;
}




bool Shader::CreateShaderProgramFromSource(const std::string &_vertexShaderSource, const std::string &_fragmentShaderSource)
{
	isInitialized = false;

	GLuint vertexShader;  

	if (!CreateVertexShader(_vertexShaderSource.c_str(), vertexShader))
	{
		return false;
	}

	GLuint fragmentShader;  

	if (!CreateFragmentShader(_fragmentShaderSource.c_str(), fragmentShader))
	{
		glDeleteShader(vertexShader);
		return false;
	}

	shaderProgramId = glCreateProgram();   
	glAttachShader(shaderProgramId, vertexShader);   
	glAttachShader(shaderProgramId, fragmentShader);   
	glLinkProgram(shaderProgramId);   

	GLint success;  

	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);    

	if (success != GL_TRUE)             
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
		std::cout << "Failed to link vertex & fragment shaders: " << infoLog << std::endl;
	
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return false;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return isInitialized = true;
}

bool Shader::CreateShaderProgramFromFile(const std::string &_vertexShaderPath, const std::string &_fragmentShaderPath)
{
	isInitialized = false;

	std::string vertexShaderCode;

	File vertexShaderFile(_vertexShaderPath, File::READ);    
	
	if (vertexShaderFile.IsOpened() && vertexShaderFile.GetAccessMode() == File::READ)       
	{
		vertexShaderFile.ReadFile(vertexShaderCode);
	}
	else 
	{
		std::cout << "Failed to open vertex shader file: " << _vertexShaderPath << std::endl;
		return false;
	}


	std::string fragmentShaderCode;

	File fragmentShaderFile(_fragmentShaderPath, File::READ);    

	if (fragmentShaderFile.IsOpened() && fragmentShaderFile.GetAccessMode() == File::READ)       
	{
		fragmentShaderFile.ReadFile(fragmentShaderCode);
	}
	else
	{
		std::cout << "Failed to open fragment shader file: " << _fragmentShaderPath << std::endl;
		return false;
	}

	if (!CreateShaderProgramFromSource((const GLchar*)vertexShaderCode.c_str(), (const GLchar*)fragmentShaderCode.c_str()))
	{
		return false;
	}
	return isInitialized = true;
}