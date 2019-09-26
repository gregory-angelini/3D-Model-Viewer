#include "Lamp.h"



Lamp::Lamp()
{
	Init();
}


Lamp::~Lamp()
{
	Destroy();
}


void Lamp::Destroy()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}


void Lamp::Init()
{
	MeshInit();

	ShaderInit();
}




void Lamp::ShaderInit()
{
	shader = std::make_shared<Shader>();

	if (shader)    
	{
		if (!shader->CreateShaderProgramFromSource(GetVertexShaderSource(), GetFragmentShaderSource()))
		{
			throw std::runtime_error("Failed to create shader program");
		}
	}
}


std::string Lamp::GetVertexShaderSource()
{
	return "#version 330 core\n"
           "layout(location = 0) in vec3 position;\n"
           "uniform mat4 model;\n"
           "uniform mat4 projectionView;\n"
           "void main()\n"
           "{\n"
	       "gl_Position = projectionView * model * vec4(position, 1.0f);\n"
	       "}\0";
}

std::string Lamp::GetFragmentShaderSource()
{
	return "#version 330 core\n"
		   "out vec4 fragColor;\n"
	       "uniform vec3 lightColor;\n"
	       "void main()\n"
	       "{\n"
		   "fragColor = vec4(lightColor, 1.0f);\n"
	       "}\0";
}


void Lamp::Draw(std::shared_ptr<Shader> _shader)
{
	if (_shader)
	{
		_shader->Bind();

		_shader->SetValue("model", state.model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
}

void Lamp::Update(float _delta)
{
	state.t += _delta * state.moveSpeed;     

	float x = sin(state.t) * state.radius;
	float z = cos(state.t) * state.radius;

	state.position = glm::vec3(x, state.position.y, z);     

	state.model = glm::identity<glm::mat4>();
	state.model = glm::translate(state.model, state.position);
	state.model = glm::scale(state.model, glm::vec3(0.2f));
}



void Lamp::Draw(const glm::mat4 &_projectionView)
{
	if (shader)   
	{
		shader->Bind();    

		shader->SetValue("projectionView", _projectionView);
		shader->SetValue("model", state.model);
		shader->SetValue("lightColor", state.color);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
}

void Lamp::MeshInit()
{
	float vertices[] =
	{
	  -0.5f, -0.5f, -0.5f,  
	   0.5f, -0.5f, -0.5f, 
	   0.5f,  0.5f, -0.5f,
	   0.5f,  0.5f, -0.5f,  
	  -0.5f,  0.5f, -0.5f, 
	  -0.5f, -0.5f, -0.5f,  

	  -0.5f, -0.5f,  0.5f, 
	   0.5f, -0.5f,  0.5f,  
	   0.5f,  0.5f,  0.5f, 
	   0.5f,  0.5f,  0.5f, 
	  -0.5f,  0.5f,  0.5f, 
	  -0.5f, -0.5f,  0.5f, 

	  -0.5f,  0.5f,  0.5f, 
	  -0.5f,  0.5f, -0.5f,
	  -0.5f, -0.5f, -0.5f, 
	  -0.5f, -0.5f, -0.5f,
	  -0.5f, -0.5f,  0.5f, 
	  -0.5f,  0.5f,  0.5f, 

	   0.5f,  0.5f,  0.5f, 
	   0.5f,  0.5f, -0.5f, 
	   0.5f, -0.5f, -0.5f, 
	   0.5f, -0.5f, -0.5f, 
	   0.5f, -0.5f,  0.5f,  
	   0.5f,  0.5f,  0.5f, 

	  -0.5f, -0.5f, -0.5f,  
	   0.5f, -0.5f, -0.5f, 
	   0.5f, -0.5f,  0.5f,  
	   0.5f, -0.5f,  0.5f,  
	  -0.5f, -0.5f,  0.5f,  
	  -0.5f, -0.5f, -0.5f,  

	  -0.5f,  0.5f, -0.5f,  
	   0.5f,  0.5f, -0.5f,  
	   0.5f,  0.5f,  0.5f, 
	   0.5f,  0.5f,  0.5f,  
	  -0.5f,  0.5f,  0.5f,  
	  -0.5f,  0.5f, -0.5f, 
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
