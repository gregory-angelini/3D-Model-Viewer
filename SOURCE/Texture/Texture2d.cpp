#include <Texture/Texture2d.h>
#include <SOIL.h> 
#include <File/File.h>
#include <Common/Common.h>
#include <iostream>
#include <Window/Window.h>


Texture2d::Texture2d() 
{}



Texture2d::~Texture2d()
{
	Destroy();
}

Texture2d::Type Texture2d::GetType()const
{
	return type;
}


Texture2d::Role Texture2d::GetRole()const
{
	return role;
}


void Texture2d::SetRole(Texture2d::Role _role)
{
	role = _role;
}



glm::vec2 Texture2d::GetImageSize()const
{
	return imageSize;
}



GLuint Texture2d::GetId()const
{
	return textureId;
}


bool Texture2d::IsExist()const
{
	return isInitialized;
}


void Texture2d::Destroy()
{
	if (IsExist())   
	{
		glDeleteTextures(1, &textureId);            
		isInitialized = false;
	}
}


GLint Texture2d::_GetTextureMaxSize()
{
	GLint maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	return maxSize;
}

Texture2d::FilteringMode Texture2d::GetFilteringMode()const
{
	return filteringMode;
}


void Texture2d::SetFilteringMode(Texture2d::FilteringMode _mode)
{
	if (filteringMode != _mode)
	{
		filteringMode = _mode;   

		switch (_mode)
		{
			case POINT_FILTERING:
			{
				magFilter = GL_NEAREST;
				minFilter = GL_NEAREST;
				ApplyFilteringMode();
				break;
			}

			case BILINEAR_FILTERING:
			{
				magFilter = GL_LINEAR;
				minFilter = GL_LINEAR;
				ApplyFilteringMode();
				break;
			}

			case TRILINEAR_FILTERING:
			case ANISOTROPY_FILTERING:
			{
				magFilter = GL_LINEAR;
				minFilter = GL_LINEAR_MIPMAP_LINEAR;
				ApplyFilteringMode();
				break;
			}
		}
	}
}


void Texture2d::SetName(std::string _name)
{
	name = _name;
}


std::string Texture2d::GetName()const
{
	return name;
}



void Texture2d::CreateDepthBuffer(glm::vec2 _size)
{
	Destroy();   

	imageSize = glm::vec2(_size.x, _size.y);

	textureSize = imageSize;

	type = Type::TEXTURE_2D;  

	glGenTextures(1, &textureId);                       
	glBindTexture(type, (GLuint)textureId);                                 
	isInitialized = true;

	ApplyFilteringMode();    
 
	glTexImage2D(type,   
		         0,             
		         GL_DEPTH_COMPONENT32,       
		         textureSize.x, textureSize.y,   
		         0,     
		         GL_DEPTH_COMPONENT,        
		         GL_FLOAT,        
		         NULL);            

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindTexture(type, 0);  
}


void Texture2d::CreateColorBufferRGBA(glm::vec2 _size, bool _gammaCorrection)
{
	Destroy();   

	imageSize = glm::vec2(_size.x, _size.y);

	textureSize = imageSize;

	type = Type::TEXTURE_2D;  

	glGenTextures(1, &textureId);                       
	glBindTexture(type, (GLuint)textureId);                                 
	isInitialized = true;

	ApplyFilteringMode();    

	glTexImage2D(type,   
		         0,             
				 _gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA,       
		         textureSize.x, textureSize.y,   
		         0,     
		         GL_RGBA,        
		         GL_FLOAT,        
		         NULL);            

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);                                    
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);            

	glBindTexture(type, 0);  
}




bool Texture2d::CreateCubeMap(const std::vector<std::string> &_order, bool _gammaCorrection)
{
	Destroy();   
	 
	if (_order.size() >= 6)      
	{
		type = Type::CUBE_MAP;  	

		glGenTextures(1, &textureId);                       
		glBindTexture(type, (GLuint)textureId);                                 		
		
		int imageWidth, imageHeight;   

		for (unsigned int i = 0; i < 6; i++)
		{
			unsigned char* image = SOIL_load_image(_order[i].c_str(),     
												   &imageWidth,    
												   &imageHeight,    
												   0,    
												   SOIL_LOAD_RGBA);     

			if (image)    
			{
				imageSize = glm::vec2(imageWidth, imageHeight);   

				if (imageWidth > _GetTextureMaxSize() || imageHeight > _GetTextureMaxSize())          
				{ 
					std::cout << "Failed to create texture [" << _order[i] << "]. Image size greater than " << _GetTextureMaxSize() << std::endl;
					SOIL_free_image_data(image);   
					glDeleteTextures(1, &textureId);            
					return false;
				}

				textureSize = imageSize;
			
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,  
						     0,            
					         _gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA,       
					         textureSize.x, textureSize.y,   
						     0,             
						     GL_RGBA,           
						     GL_UNSIGNED_BYTE,              
						     image);       
			
				SOIL_free_image_data(image);   
			}
			else
			{
				std::cout << "Failed to load image [" << _order[i] << "]. Error code: " << SOIL_last_result() << std::endl;
				glDeleteTextures(1, &textureId);            
				return false;
			}
		}

		isInitialized = true;

		ApplyFilteringMode();    

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);                                    
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);            
		glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);          

		glBindTexture(type, 0);  
		return true;
	}
	return false;
}



bool Texture2d::Create(const std::string &_path, bool _gammaCorrection)
{
	Destroy();   

	int imageWidth, imageHeight;
	unsigned char* image = SOIL_load_image(_path.c_str(),     
			                               &imageWidth,    
			                               &imageHeight,    
			                               0,    
			                               SOIL_LOAD_RGBA);     

	if (image)    
	{
		imageSize = glm::vec2(imageWidth, imageHeight);

		if (imageWidth > _GetTextureMaxSize() || imageHeight > _GetTextureMaxSize())          
		{
			std::cout << "Failed to create texture [" << _path << "]. Image size greater than " << _GetTextureMaxSize() << std::endl;
			SOIL_free_image_data(image);   
			return false;
		}

		textureSize = imageSize;

		type = Type::TEXTURE_2D;  

		glGenTextures(1, &textureId);                       
		glBindTexture(type, (GLuint)textureId);                                 
		isInitialized = true;

		ApplyFilteringMode();    
	
		glTexImage2D(type,  
					 0,            
			         _gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA,      
			         textureSize.x, textureSize.y,   
					 0,             
					 GL_RGBA,           
					 GL_UNSIGNED_BYTE,             
					 image);        

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);                                    
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);            
		
		SOIL_free_image_data(image);   
		glBindTexture(type, 0);  
		return true;
	} 
	else 
	{
		std::cout << "Failed to load image [" << _path << "]. Error code: " << SOIL_last_result() << std::endl;
	}
	return false;
}




void Texture2d::ApplyFilteringMode()
{
	if (IsExist())   
	{
		glBindTexture(type, textureId);                  

		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magFilter);     
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minFilter);     

		if (filteringMode == ANISOTROPY_FILTERING)
		{
			if (Window::_GetTextureMaxAnisotropy() != 0)    
			{
				glTexParameteri(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, Window::_GetTextureMaxAnisotropy());  

				glGenerateMipmapEXT(type);        

				glTexSubImage2D(type,   
					            0,               					            
					            0, 0,     
					            0, 0,  
					            GL_RGBA,    
					            GL_UNSIGNED_BYTE,        
					            NULL);       
			}
		}
		else if (filteringMode == TRILINEAR_FILTERING)
		{
			if (Window::_GetTextureMaxAnisotropy() != 0)     
			{
				glTexParameteri(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);  
			}

			glGenerateMipmapEXT(type);        

			glTexSubImage2D(type,   
				            0,               
				            0, 0,  
				            0, 0,  
				            GL_RGBA,    
				            GL_UNSIGNED_BYTE,        
				            NULL);       
		}
		else
		{
			if (Window::_GetTextureMaxAnisotropy() != 0)     
			{
				glTexParameteri(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);  
			}
		}
	}
}
