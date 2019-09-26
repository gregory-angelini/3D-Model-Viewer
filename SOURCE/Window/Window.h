#pragma once
#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Color/RGBA/RGBA.h>
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Texture/Texture2d.h>


class Window
{
	public: enum CullFaceMode
	{
		CULL_DISABLE = 0,   
		CULL_BACK_FACES = GL_BACK,   
		CULL_FRONT_FACES = GL_FRONT   
	};

	public: enum MultisamplingMode
	{
		DISABLE = 0,  
		MULTI_SAMPLING_2X = 2,
		MULTI_SAMPLING_4X = 4,
		MULTI_SAMPLING_8X = 8
	};

	public: class Define
	{
		public: std::string title = "Application";  
		public: glm::vec2 clientAreaSize = glm::vec2(1024, 768);       
		public: RGBA backgroundColor = RGBA(0, 0, 0, 255);     
		public: bool wireframeEnable = false;       
	    public: bool zBufferEnable = true;        
		public: bool blendEnable = false;      
	    public: bool stencilEnable = false;      

		public: MultisamplingMode multisamplingMode = MultisamplingMode::DISABLE;           
		public: bool multisamplingATCEnable = false;             
		public: CullFaceMode cullFaceMode = CullFaceMode::CULL_BACK_FACES;   

		public: Define()
		{ }
	};

	private: std::string windowTitle = "Application";  
	private: glm::vec2 clientAreaSize = glm::vec2(1024, 768);       
	private: glm::vec2 viewportSize = glm::vec2(0.0f);    
	private: RGBA backgroundColor = RGBA(0, 0, 0, 255);     
    private: bool wireframeEnable = false;       
	private: MultisamplingMode multisamplingMode = MultisamplingMode::DISABLE;           
	private: bool multisamplingATCEnable = false;             
	private: CullFaceMode cullFaceMode = CullFaceMode::CULL_BACK_FACES;    
    private: bool zBufferEnable = true;        
    private: bool blendEnable = false;      
	private: bool stencilEnable = false;      
			  
    private: bool isInitialized = false;     
	private: GLFWwindow* window = 0;    
    private: static int textureMaxAnisotropy;        
	private: static std::list<std::string> glExtensions;   
	private: GLbitfield clearActions;         
	private: GLfloat deltaTime = 0.0f;     
	private: GLfloat lastSystemTime = 0.0f;     
	private: GLfloat currentSystemTime = 0.0f;   
    private: glm::vec2 lastCursorPosition;      
	private: bool keys[1024];   
	private: static int maxCombinedTextureImageUnits;        
	private: static std::vector <std::string> diffuseMapNames;          
	private: static std::vector <std::string> specularMapNames;          
	private: static std::vector <std::string> normalMapNames;         
	private: static std::vector <std::string> shadowMapNames;         
    private: static std::vector <std::string> skyboxMapNames;         
 
	public: Window();


	public: virtual ~Window();


	public: void SetWindowTitle(const std::string &_title);


	protected: virtual bool Init(const Window::Define &_define);


	protected: virtual void Destroy();

		
	private: bool IsGLExtensionSupport(const std::string &_name);


	private: void InitGLExtensions();


	public: glm::vec2 GetClientAreaSize()const;


	public: glm::vec2 GetViewportSize()const;


	public: void SetBlendEnable(bool _enable);


	public: void LockCursor();


	private: static void KeyboardEventHandlerStatic(GLFWwindow* _window, int _key, int _scancode, int _action, int _mode);


	public: virtual void KeyboardEventHandler(int _key, int _action, int _mode);


	private: static void MouseMoveEventHandlerStatic(GLFWwindow* _window, double _xPos, double _yPos);


	public: virtual void MouseMoveEventHandler(glm::vec2 _position);


	private: static void MouseWheelEventHandlerStatic(GLFWwindow* _window, double _xOfsset, double _yOffset);


	public: virtual void MouseWheelEventHandler(int _offset);


	protected: virtual void Draw();

 
	public: void Loop();
	

	public: bool Frame();

			
	public: virtual void Update(float _delta);


	public: void SetWireframeEnable(bool _enable);


	public: static int _GetTextureMaxAnisotropy();


	public: static int _GetMaxCombinedTextureImageUnits();


	public: static const std::vector<std::string>& _GetShadowMapNames();

			
	public: static const std::vector<std::string>& _GetSkyboxMapNames();


	public: static const std::vector<std::string>& _GetDiffuseMapNames();


	public: static const std::vector<std::string>& _GetSpecularMapNames();

			
	public: static const std::vector<std::string>& _GetNormalMapNames();


	private: static void _InitMultitexturedUnits();


	public: void ClearScreen();


	public: void SetBackgroundColor(RGBA _color);
			
			
	public: void SetZBufferEnable(bool _enable);


	public: bool IsZBufferEnabled()const;

			
	public: GLFWwindow* GetHandler();


	public: float GetDeltaTime();


	public: glm::vec2 GetLastCursorPosition()const;


	public: bool IsKeyPressed(int _key);

			
	public: bool IsKeyReleased(int _key);

		
	public: void SetStencilEnable(bool _enable);


	public: void SetCullFaceMode(CullFaceMode _cullFaceMode);


	public: void SetMultisamplingATCEnable(bool _enalbe);


	public: void SetMultisamplingEnable(bool _enable);
};
