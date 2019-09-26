#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <list>
#include <Math/glm.h>



class Camera
{
	public: enum Movement
	{
		FORWARD, 
		BACKWARD, 
		LEFT, 
		RIGHT, 
		UP, 
		DOWN  
	};

	public: enum RotationMode
	{
		FIRST_PERSON,                                       
		OBJECT_VIEWER                                           
	};


	public: enum MovementMode
	{
		FLIGHT,           
		SPECTATOR,           
		OVERLAND      
	};

	private: float fov = glm::radians(45.0f);                              
	private: float viewportRatio = 1.0f;               
	private: float zNear = 0.1f;     
	private: float zFar = 100.0f;     

	private: MovementMode movementType = MovementMode::SPECTATOR;     
	private: glm::vec3 position = glm::vec3(0.0f);    
	private: float movementSpeed = 3.0f;      
	private: glm::quat rotation = glm::init<glm::quat>::identity();    
	private: RotationMode rotationMode = RotationMode::OBJECT_VIEWER;    
	private: float horizontalSensitivity = 0.2f, verticalSensitivity = 0.2f;        
	private: bool reversePitchRotationEnable = false;             
	private: bool reverseYawRotationEnable = true;             
	
    private: bool rotationEnable = true;       	
	private: bool pitchRotationEnable = true;         
	private: bool yawRotationEnable = true;         	
    private: bool rollRotationEnable = true;         			
	private: bool pitchClampEnable = true;            
    private: float pitchMin = -glm::half_pi<float>();                   
	private: float pitchMax = glm::half_pi<float>();                   
	private: bool yawClampEnable = false;            
	private: float yawMin = -glm::half_pi<float>();                       
	private: float yawMax = glm::half_pi<float>();                       
	private: bool rollClampEnable = false;            
	private: float rollMin = -glm::half_pi<float>();                       
	private: float rollMax = glm::half_pi<float>();                       
	private: bool zoomEnable = false;                   
	private: float zoomSpeed = 0.3f;                
	private: float zoomNear;     
	private: float zoomFar;     

	private: float distanceToViewingPoint;           
	private: glm::vec3 viewingPoint = glm::vec3(0.0f);      
	private: glm::vec3 viewingPointYAxis = glm::init<glm::vec3>::unitY();        

	private: glm::mat4 viewMatrix = glm::init<glm::mat4>::identity();                 
	private: glm::mat4 projectionMatrix = glm::init<glm::mat4>::identity();  
	private: glm::mat3 rotationMatrix = glm::init<glm::mat4>::identity();             															   

	private: glm::vec3 look = glm::vec3(0.0f);         
	private: float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;        

	public: Camera();


	public: Camera(float _viewportRatio);


	public: void SetZoomEnable(bool _enable);


	public: bool IsZoomEnabled()const;
	
			
	public: void Zoom(float _zoom);

	public: void SetDistanceToViewingPoint(float _distance);


	public: float GetDistanceToViewingPoint()const;


	private: void ClampRadians180(float& _radians);


	private: void AccumPitch(float _radians);


	private: void AccumYaw(float _radians);


	private: void AccumRoll(float _radians);


	private: void YawClamp(float& _radians);


	public: void SetYawMin(float _radians);


	public: float GetYawMin()const;


	public: void SetYawMax(float _radians);


	public: float GetYawMax()const;
 
			
	private: void PitchClamp(float& _radians);


	public: void SetPitchMin(float _radians);


	public: float GetPitchMin()const;


	public: void SetPitchMax(float _radians);


	public: float GetPitchMax()const;


	private: void RollClamp(float& _radians);


	public: void SetRollMin(float _radians);


	public: float GetRollMin()const;


	public: void SetRollMax(float _radians);


	public: float GetRollMax()const;


	public: void ResetRoll();


	public: glm::quat GetRotation()const;

			
	public: glm::mat4 GetRotationMatrix()const;


	public: void LookAt(glm::vec3 _from, glm::vec3 _to, glm::vec3 _up);


	public: void LookAt(glm::vec3 _target);


	public: bool IsRollClampEnabled()const;


	public: void SetRollClampEnable(bool _enable);


	public: void SetPitchClampEnable(bool _enable);


	public: bool IsPitchClampEnabled()const;


	public: void SetYawClampEnable(bool _enable);


	public: bool IsYawClampEnabled()const;


	public: void SetRotation(glm::quat _rotation);

			
	public: virtual void DisplaceRotation(glm::quat _rotation);


	public: void SetHorizontalSensitivity(float _sensitivity);


	public: float GetHorizontalSensitivity()const;


	public: void SetVerticalSensitivity(float _sensitivity);


	public: float GetVerticalSensitivity()const;


	public: void SetReverseYawRotationEnable(bool _enable);


	public: bool IsReverseYawRotationEnabled()const;


	public: void SetReversePitchRotationEnable(bool _enable);


	public: bool IsReversePitchRotationEnabled()const;


	public: void SetRotationMode(RotationMode _rotationMode);


	public: RotationMode GetRotationMode()const;


	public: void SetViewingPoint(glm::vec3 _point);


	public: glm::vec3 GetViewingPoint()const;


	public: void SetViewingPointYAxis(glm::vec3 _axis);


	public: glm::vec3 GetViewingPointYAxis()const;


	public: void SetRotationEnable(bool _enable);


	public: bool IsRotationEnabled()const;


	public: void SetPitchRotationEnable(bool _enable);


	public: bool IsPitchRotationEnabled()const;


	public: void ReverseLook();


	public: glm::vec3 GetLook()const;


	public: void Strafe(float _units);


	public: void Fly(float _units);


	public: void Walk(float _units);

			
	public: void Move(Movement _direction, float _deltaTime);


	public: glm::vec3 GetPosition()const;


	public: void SetPosition(glm::vec3 _position);


	public: void DisplacePosition(glm::vec3 _offset);


	public: glm::mat4 GetViewMatrix()const;


	public: glm::mat4 GetProjectionMatrix()const;

			
	private: void UpdateViewMatrix();


	private: void UpdateViewMatrixTranslation();


	private: void UpdateProjectionMatrix();

			
	public: void SetViewportRatio(float _viewportRatio);


	public: void SetZoomNear(float _zoomNear);


	public: float GetZoomNear()const;


	public: void SetZoomFar(float _zoomFar);


	public: float GetZoomFar()const;


	public: float GetZoomSpeed()const;


	public: void SetZoomSpeed(float _zoomSpeed);

			
	public: void SetFov(float _fov);


	public: float GetFov()const;

			
	public: void SetMovementMode(MovementMode _movementType);


	public: MovementMode GetMovementMode()const;


	public: void SetMovementSpeed(float _speed);


	public: float GetMovementSpeed()const;


	public: virtual void MoveTo(glm::vec3 _direction, float _units);


	public: void UpdateTrackballFirstPersonWithOverland(float _pitch, float _yaw, float _roll);


	public: void UpdateTrackballFirstPersonWithFlight(float _pitch, float _yaw, float _roll);


	public: void UpdateTrackballFirstPersonWithSpectator(float _pitch, float _yaw, float _roll);

			
	public: void UpdateTrackballObjectViewer(float _pitch, float _yaw, float _roll);


	public: virtual void MouseMoveEventHandler(float _xPosOffset, float _yPosOffset);
}; 
