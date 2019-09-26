#include "Skybox.h"



Skybox::Skybox()
{
	Init();
}


Skybox::~Skybox()
{
	Destroy();
}


void Skybox::Destroy()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}


void Skybox::MeshInit()
{
	float vertices[] =
	{
		-1.0f,  1.0f, -1.0f,
	    -1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
	    -1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};


	glGenBuffers(1, &VBO);    
	glBindBuffer(GL_ARRAY_BUFFER, VBO);   
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);     


	glGenVertexArrays(1, &VAO);     

	glBindVertexArray(VAO);   
	glBindBuffer(GL_ARRAY_BUFFER, VBO);   

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);   
	glBindVertexArray(0);   
}


void Skybox::ShaderInit()
{
	shader = std::make_shared<Shader>();

	if (shader)    
	{
		if (!shader->CreateShaderProgramFromFile("shaders/skybox/skybox.vs", "shaders/skybox/skybox.fs"))
		{
			throw std::runtime_error("Failed to create shader program");
		}
	}
}


void Skybox::Init()
{
	texture = std::make_shared<Texture2d>();

	MeshInit();

	ShaderInit();
}

std::shared_ptr<Texture2d> Skybox::GetTexture()const
{
	return texture;
}


void Skybox::Create(const std::vector<std::string> &_order)
{ 
	if (texture)
	{
		texture->CreateCubeMap(_order);
		texture->SetRole(Texture2d::Role::SKYBOX);
	}
}


void Skybox::SetScale(glm::vec3 _scale)
{
	model = glm::identity<glm::mat4>();
	model = glm::scale(model, _scale);
}


void Skybox::Draw(const glm::mat4 &_projectionView)
{
	if (shader)   
	{
		shader->Bind();    

		shader->SetValue("projectionView", _projectionView);
		shader->SetValue("model", model);

		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->GetId());  

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
}