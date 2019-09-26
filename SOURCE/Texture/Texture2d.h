#pragma once
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Texture2d
{
	public: enum FilteringMode
	{
		POINT_FILTERING,   
		BILINEAR_FILTERING,  
		TRILINEAR_FILTERING,  
		ANISOTROPY_FILTERING 
	};
		

	public: enum Type
	{
		TEXTURE_2D = GL_TEXTURE_2D,   
		CUBE_MAP = GL_TEXTURE_CUBE_MAP  
	};

	public: enum Role
	{
		SHADOW_MAP_DIRECTIONAL_LIGHT_1 = 0,       
		UNDEFINED,  
		DIFFUSE,   
		SPECULAR,   
		NORMAL,  
		SKYBOX  
	};

	private: glm::vec2 imageSize;    
	private: glm::vec2 textureSize;      
	private: GLuint textureId = 0;  
	private: FilteringMode filteringMode = POINT_FILTERING;   
	private: int magFilter = GL_NEAREST;    
	private: int minFilter = GL_NEAREST;    
	private: Role role = Role::UNDEFINED;  
	private: Type type = Type::TEXTURE_2D;  
	private: std::string name;  
	private: bool isInitialized = false;     
			 

	public: Texture2d();

	
	public: ~Texture2d();


	public: Type GetType()const;


	public: Role GetRole()const;


    public: void SetRole(Role _role);


	public: void CreateColorBufferRGBA(glm::vec2 _size, bool _gammaCorrection = true);

			
	public: void CreateDepthBuffer(glm::vec2 _size);


	public: bool Create(const std::string &_path, bool _gammaCorrection = false);


	public: glm::vec2 GetImageSize()const;


	public: bool IsExist()const;


	public: bool CreateCubeMap(const std::vector<std::string> &_order, bool _gammaCorrection = true);


	public: GLuint GetId()const;


	public: void Destroy();

	
	public: static GLint _GetTextureMaxSize();


	public: void SetFilteringMode(FilteringMode _mode);


	public:FilteringMode GetFilteringMode()const;


	private: void ApplyFilteringMode();

	
	public: std::string GetName()const;


	public: void SetName(std::string _name);
};

