#include "Window.h"
#include <string>

int Window::textureMaxAnisotropy = 0;
int Window::maxCombinedTextureImageUnits = 0;
std::list<std::string> Window::glExtensions;
std::vector <std::string> Window::diffuseMapNames;
std::vector <std::string> Window::specularMapNames;
std::vector <std::string> Window::normalMapNames;
std::vector <std::string> Window::shadowMapNames;
std::vector <std::string> Window::skyboxMapNames;


Window::Window() 
{
	for (int i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
}

const std::vector<std::string>& Window::_GetSkyboxMapNames()
{
	return skyboxMapNames;
}


const std::vector<std::string>& Window::_GetShadowMapNames()
{
	return shadowMapNames;
}


const std::vector<std::string>& Window::_GetDiffuseMapNames()
{
	return diffuseMapNames;
}


const std::vector<std::string>& Window::_GetSpecularMapNames()
{
	return specularMapNames;
}


const std::vector<std::string>& Window::_GetNormalMapNames()
{
	return normalMapNames;
}


void Window::_InitMultitexturedUnits()
{
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureImageUnits);
	std::cout << "Max multitextured units supported: " << maxCombinedTextureImageUnits << std::endl;

	int multitexturedUnits = _GetMaxCombinedTextureImageUnits();

	diffuseMapNames.resize(multitexturedUnits);    

	for (int i = 0; i < diffuseMapNames.size(); i++)
	{
		diffuseMapNames[i] = "material.diffuse[" + std::to_string(i) + "]";
	}


	specularMapNames.resize(multitexturedUnits);    

	for (int i = 0; i < specularMapNames.size(); i++)
	{
		specularMapNames[i] = "material.specular[" + std::to_string(i) + "]";
	}

	normalMapNames.resize(multitexturedUnits);    

	for (int i = 0; i < normalMapNames.size(); i++)
	{
		normalMapNames[i] = "material.normal[" + std::to_string(i) + "]";
	}


	skyboxMapNames.resize(1);    

	skyboxMapNames[0] = "skybox";


	shadowMapNames.resize(1);    

	shadowMapNames[Texture2d::Role::SHADOW_MAP_DIRECTIONAL_LIGHT_1] = "depthMap";
}


Window::~Window()
{
	Destroy();
}

glm::vec2 Window::GetViewportSize()const
{
	return viewportSize;
}



glm::vec2 Window::GetClientAreaSize()const
{
	return clientAreaSize;
}



void Window::SetWindowTitle(const std::string &_title)
{
	windowTitle = _title;

	if (window)
	{
		glfwSetWindowTitle(window, windowTitle.c_str());
	}
}




bool Window::Init(const Window::Define &_define)
{
	if (!isInitialized)
	{
		glfwInit();    

		clientAreaSize = _define.clientAreaSize;
		windowTitle = _define.title;
		multisamplingMode = _define.multisamplingMode;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);   
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);          
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);    
		glfwWindowHint(GLFW_SAMPLES, multisamplingMode);       

		window = glfwCreateWindow(clientAreaSize.x, clientAreaSize.y,       
			                      windowTitle.c_str(),
			                      nullptr,        
			                      nullptr);

		if (window == nullptr)
		{
			glfwTerminate();    
			throw std::runtime_error("Failed to create GLFW window");
			return false;
		}

		glfwMakeContextCurrent(window);            

		glewExperimental = GL_TRUE;       

		if (glewInit() != GLEW_OK)   
		{
			glfwDestroyWindow(window);  
			glfwTerminate();    
			throw std::runtime_error("Failed to initialize GLEW");
			return false;
		}

		const unsigned char *version = glGetString(GL_VERSION);
		std::cout << "OpenGL version: " << version << std::endl;

		InitGLExtensions();   

		GLint maxVertexAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
		std::cout << "Max vertex attributes supported: " << maxVertexAttributes << std::endl;

		_InitMultitexturedUnits();  

		glfwSetWindowUserPointer(window, this);     
		glfwSetKeyCallback(window, KeyboardEventHandlerStatic);     

		glfwSetCursorPosCallback(window, MouseMoveEventHandlerStatic);     
		glfwSetScrollCallback(window, MouseWheelEventHandlerStatic);      

		isInitialized = true;

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);               
		viewportSize = glm::vec2(width, height);   
		 
		glViewport(0, 0, width, height);       

		clearActions = GL_COLOR_BUFFER_BIT;                    

		SetWireframeEnable(_define.wireframeEnable);   
		 
		SetZBufferEnable(_define.zBufferEnable);
		 
		SetBackgroundColor(_define.backgroundColor);

		SetBlendEnable(_define.blendEnable);
		  
		SetCullFaceMode(_define.cullFaceMode);

		SetStencilEnable(_define.stencilEnable);
		 
		SetMultisamplingEnable(multisamplingMode > 0);
		 
		SetMultisamplingATCEnable(_define.multisamplingATCEnable);
		  
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		lastCursorPosition = glm::vec2(xPos, yPos);
		return true;
	}
	return false;
}

void Window::SetMultisamplingEnable(bool _enable)
{
	if (isInitialized)
	{
		if (_enable) { glEnable(GL_MULTISAMPLE); }
		else { glDisable(GL_MULTISAMPLE); }
	}
}


void Window::SetMultisamplingATCEnable(bool _enable)
{
	multisamplingATCEnable = _enable;

	if (isInitialized)
	{
		if (multisamplingATCEnable)
		{
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
		else
		{
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
	}
}

void Window::SetCullFaceMode(CullFaceMode _cullFaceMode)
{
	cullFaceMode = _cullFaceMode;
	
	if (isInitialized)
	{
		if (cullFaceMode > 0)
		{
			glEnable(GL_CULL_FACE);   
			glCullFace(cullFaceMode);     
		}
		else
		{
			glDisable(GL_CULL_FACE);   
		}
	}
}


void Window::SetStencilEnable(bool _enable)
{
	stencilEnable = _enable;

	if (isInitialized)
	{
		if (stencilEnable)
		{
			glEnable(GL_STENCIL_TEST);                         
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			clearActions |= GL_STENCIL_BUFFER_BIT;     
		}
		else
		{
			glDisable(GL_STENCIL_TEST);   
			
			clearActions &= ~GL_STENCIL_BUFFER_BIT;                      
		}
	}
}


void Window::SetBlendEnable(bool _enable)
{
	blendEnable = _enable;

	if (isInitialized)
	{
		if (blendEnable)
		{
			glEnable(GL_BLEND);   

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                                   
			glBlendEquation(GL_FUNC_ADD);        
		}
		else
		{
			glDisable(GL_BLEND);   
		}
	}
}

void Window::LockCursor()
{
	glfwSetInputMode(GetHandler(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);      

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	lastCursorPosition = glm::vec2(xPos, yPos);
}


void Window::SetWireframeEnable(bool _enable)
{
	wireframeEnable = _enable;

	if (isInitialized)
	{
		if (_enable)      
		{
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);

			glDisable(GL_TEXTURE_2D);  
		}
		else    
		{
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);

			glEnable(GL_TEXTURE_2D);  
		}
	} 
}


void Window::SetBackgroundColor(RGBA _color)
{
	backgroundColor = _color;
}


void Window::Loop()
{
	if (isInitialized)
	{
		while (Frame())     
		{}
	}
}

bool Window::Frame()
{
	if(!glfwWindowShouldClose(window))    
	{
		glfwPollEvents();       

		ClearScreen();  

		Draw();  

		Update(deltaTime);  

		glfwSwapBuffers(window);                        
		
		currentSystemTime = glfwGetTime();
		deltaTime = currentSystemTime - lastSystemTime;
		lastSystemTime = currentSystemTime;
		return true;
	}
	return false;
}


glm::vec2 Window::GetLastCursorPosition()const
{
	return lastCursorPosition;
}


float Window::GetDeltaTime()
{
	return deltaTime;
}


void Window::Draw()
{}


GLFWwindow* Window::GetHandler()
{
	return window;
}


void Window::Update(float _delta)
{}



void Window::ClearScreen()
{
	glClearColor(backgroundColor.red,
		         backgroundColor.green,
		         backgroundColor.blue,
		         backgroundColor.alpha);

	glClear(clearActions);             
}



void Window::KeyboardEventHandlerStatic(GLFWwindow* _window, int _key, int _scancode, int _action, int _mode)
{
	Window* it = static_cast<Window*>(glfwGetWindowUserPointer(_window));
	it->KeyboardEventHandler(_key, _action, _mode);
}


void Window::KeyboardEventHandler(int _key, int _action, int _mode)
{
	if (_action == GLFW_PRESS)
	{
		keys[_key] = true;
	}
	else if (_action == GLFW_RELEASE)
	{
		keys[_key] = false;
	}
}

bool Window::IsKeyPressed(int _key)
{
	return keys[_key];
}


bool Window::IsKeyReleased(int _key)
{
	return !keys[_key];
}

void Window::MouseMoveEventHandlerStatic(GLFWwindow* _window, double _xPos, double _yPos)
{
	Window* it = static_cast<Window*>(glfwGetWindowUserPointer(_window));
	it->MouseMoveEventHandler(glm::vec2(_xPos, _yPos));
}



void Window::MouseWheelEventHandlerStatic(GLFWwindow* _window, double _xOffset, double _yOffset)
{
	Window* it = static_cast<Window*>(glfwGetWindowUserPointer(_window));
	it->MouseWheelEventHandler((int)_yOffset);
}


void Window::MouseMoveEventHandler(glm::vec2 _position)
{
	lastCursorPosition = glm::vec2(_position.x, _position.y);
}


void Window::MouseWheelEventHandler(int _offset)
{}


void Window::Destroy()
{
	if (isInitialized)
	{
		glfwDestroyWindow(window);  
		window = NULL;

		glfwTerminate();    

		isInitialized = false;
	}
}

int Window::_GetTextureMaxAnisotropy()
{
	return textureMaxAnisotropy;
}


int Window::_GetMaxCombinedTextureImageUnits()
{
	return maxCombinedTextureImageUnits;
}


bool Window::IsGLExtensionSupport(const std::string &_name)
{
	for (auto it : glExtensions)    
	{
		if (it == _name)   
		{
			return true;
		}
	}
	return false;
}



void Window::InitGLExtensions()
{
	GLint amountOfextensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &amountOfextensions);    

	for (int i = 0; i < amountOfextensions; i++)
	{
		glExtensions.push_back((const char*)glGetStringi(GL_EXTENSIONS, i));
	}

	if (IsGLExtensionSupport("GL_EXT_texture_filter_anisotropic"))    
	{
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &textureMaxAnisotropy);

		std::cout << "Texture filter anisotropic is supported. Texture max anisotropy: " << textureMaxAnisotropy << std::endl;
	}
	else    
	{
		std::cout << "Texture filter anisotropic is not supported" << std::endl;
	}
}



void Window::SetZBufferEnable(bool _enable)
{
	zBufferEnable = _enable;

	if (isInitialized)
	{
		if (zBufferEnable)
		{
			glEnable(GL_DEPTH_TEST);                 
			glDepthFunc(GL_LESS);         

			clearActions |= GL_DEPTH_BUFFER_BIT;     
		}
		else
		{
			glDisable(GL_DEPTH_TEST);   
			clearActions &= ~GL_DEPTH_BUFFER_BIT;                      
		}
	}
}


bool Window::IsZBufferEnabled()const
{
	return zBufferEnable;
}


