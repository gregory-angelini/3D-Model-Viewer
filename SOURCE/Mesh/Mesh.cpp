#include <Mesh/Mesh.h>
#include <cstddef>
#include <Window/Window.h>


Mesh::Mesh()
{}


Mesh::~Mesh()
{
	Destroy();
}


bool Mesh::IsExist()const
{
	return isInitialized;
}


void Mesh::Destroy()
{
	if (IsExist())
	{
		vertices.clear();
		indices.clear();
		textures.clear();

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		VAO = VBO = EBO = NULL;
		isInitialized = false;
	}
}

void Mesh::Create(const std::vector<Vertex> &_vertices, const std::vector<unsigned int> &_indices, const std::vector<std::shared_ptr<Texture2d>> &_textures)
{
	Destroy();    

	vertices = _vertices;
	indices = _indices;
	textures = _textures;

	glGenVertexArrays(1, &VAO);   
	glGenBuffers(1, &VBO);   
	glGenBuffers(1, &EBO);   

	glBindBuffer(GL_ARRAY_BUFFER, VBO);   
	glBufferData(GL_ARRAY_BUFFER,         
		         vertices.size() * sizeof(Vertex),      
			     &vertices[0],      
		         GL_STATIC_DRAW);                       

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,    
		         indices.size() * sizeof(unsigned int),     
		         &indices[0],     
		         GL_STATIC_DRAW);  

	glBindVertexArray(VAO);   
	glBindBuffer(GL_ARRAY_BUFFER, VBO);   
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glVertexAttribPointer(0,        
		                  3,           
		                  GL_FLOAT,       
		                  GL_FALSE,              
		                  sizeof(Vertex),         
		                  (void*)0);              
	glEnableVertexAttribArray(0);               

	glVertexAttribPointer(1,        
		                  3,           
		                  GL_FLOAT,       
		                  GL_FALSE,              
		                  sizeof(Vertex),         
		                  (void*)offsetof(Vertex, normal));                            
	glEnableVertexAttribArray(1);               

	glVertexAttribPointer(2,        
		                  2,           
		                  GL_FLOAT,       
		                  GL_FALSE,              
		                  sizeof(Vertex),         
		                  (void*)offsetof(Vertex, texCoord));             
	glEnableVertexAttribArray(2);                
	
	glVertexAttribPointer(3,        
		                  3,           
		                  GL_FLOAT,       
		                  GL_FALSE,              
		                  sizeof(Vertex),         
		                  (void*)offsetof(Vertex, tangent));             
	glEnableVertexAttribArray(3);                

	
	glBindBuffer(GL_ARRAY_BUFFER, 0);    

	glBindVertexArray(0);    
	isInitialized = true;
}

void Mesh::AddTextureMap(std::shared_ptr<Texture2d> _texture)
{
	textures.push_back(_texture);
}


void Mesh::Draw(std::shared_ptr<Shader> _shader)
{
	for (int i = 0, diffuseNumber = 0, specularNumber = 0, reflectNumber = 0, normalNumber = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);           

		switch (textures[i]->GetRole())
		{
			case Texture2d::Role::DIFFUSE:
			{
				_shader->SetValue(Window::_GetDiffuseMapNames()[diffuseNumber], i);       
				diffuseNumber++;
				break;
			}

			case Texture2d::Role::SPECULAR:
			{
				_shader->SetValue(Window::_GetSpecularMapNames()[specularNumber], i);       
				specularNumber++;
				break;
			}

			case Texture2d::Role::NORMAL:
			{
				_shader->SetValue(Window::_GetNormalMapNames()[normalNumber], i);       
				normalNumber++;
				break;
			}

			case Texture2d::Role::SKYBOX:
			{
				_shader->SetValue(Window::_GetSkyboxMapNames()[0], i);       
				break;
			}
			default:  
			{
				_shader->SetValue(Window::_GetShadowMapNames()[textures[i]->GetRole()], i);       
				break;
			}
		}
		glBindTexture(textures[i]->GetType(), textures[i]->GetId());        
	}

	glBindVertexArray(VAO);  
	glDrawElements(GL_TRIANGLES,       
		           indices.size(),        
		           GL_UNSIGNED_INT,        
		           0);     
	glBindVertexArray(0);  

	glActiveTexture(GL_TEXTURE0);    
}