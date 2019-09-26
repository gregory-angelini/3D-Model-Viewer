#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <iostream>


Camera::Camera()
{
	zoomNear = fov / 5.0f;
	zoomFar = fov * 5.0f;	
	distanceToViewingPoint = std::fmin(std::fmax(distanceToViewingPoint, zoomNear), zoomFar);      
	
	UpdateViewMatrix();   
	UpdateProjectionMatrix();   
}



Camera::Camera(float _viewportRatio)
{
	viewportRatio = _viewportRatio;

	zoomNear = fov / 5.0f;
	zoomFar = fov * 5.0f;
	distanceToViewingPoint = std::fmin(std::fmax(distanceToViewingPoint, zoomNear), zoomFar);      

	UpdateViewMatrix();   
	UpdateProjectionMatrix();   
}


void Camera::ClampRadians180(float& _radians)
{
	while (_radians < -glm::pi<float>())     
	{
		_radians += glm::two_pi<float>();        
	}

	while (_radians > glm::pi<float>())     
	{
		_radians -= glm::two_pi<float>();    
	}
}


void Camera::YawClamp(float& _radians)
{
	if (yawClampEnable)    
	{
		if (yaw < yawMin)     
		{
			_radians = yawMin - (yaw - _radians);
			yaw = yawMin;
		}
		else if (yaw > yawMax)     
		{
			_radians = yawMax - (yaw - _radians);
			yaw = yawMax;
		}
	}
}


 
void Camera::SetYawMin(float _radians)
{
	yawMin = _radians;

	ClampRadians180(yawMin);
}


float Camera::GetYawMin()const
{
	return yawMin;
}


void Camera::SetYawMax(float _radians)
{
	yawMax = _radians;

	ClampRadians180(yawMax);
}


float Camera::GetYawMax()const
{
	return yawMax;
}


void Camera::SetViewportRatio(float _viewportRatio)
{
	viewportRatio = _viewportRatio;

	UpdateProjectionMatrix();
}


glm::mat4 Camera::GetViewMatrix()const
{
	return viewMatrix;
}


glm::mat4 Camera::GetProjectionMatrix()const
{
	return projectionMatrix;
}


void Camera::SetZoomEnable(bool _enable)
{
	zoomEnable = _enable;
}


bool Camera::IsZoomEnabled()const
{
	return zoomEnable;
}


glm::quat Camera::GetRotation()const
{
	return rotation;
}


void Camera::SetRotation(glm::quat _rotation)
{
	rotation = glm::normalize(_rotation);

	UpdateViewMatrix();

	pitch = yaw = roll = 0.0f;
}




void Camera::LookAt(glm::vec3 _target)
{
	LookAt(GetPosition(), _target, rotationMatrix[glm::Y]);
}


void Camera::LookAt(glm::vec3 _from, glm::vec3 _to, glm::vec3 _up)
{
	glm::mat4 matrix = glm::lookAt(_from,    
		                           _to,     
		                           _up);    

	if (rotationMode == RotationMode::OBJECT_VIEWER)    
	{
		viewingPoint = _to;     
		
		distanceToViewingPoint = glm::length(_from - _to);     
	}
	
	SetRotation(glm::quat_cast(matrix));   
}



glm::mat4 Camera::GetRotationMatrix()const
{
	return rotationMatrix;
}


void Camera::DisplaceRotation(glm::quat _rotation)
{
	SetRotation(glm::normalize(GetRotation() * _rotation));
}


void Camera::SetHorizontalSensitivity(float _sensitivity)
{
	horizontalSensitivity = _sensitivity;
}


float Camera::GetHorizontalSensitivity()const
{
	return horizontalSensitivity;
}


void Camera::SetVerticalSensitivity(float _sensitivity)
{
	verticalSensitivity = _sensitivity;
}



float Camera::GetVerticalSensitivity()const
{
	return verticalSensitivity;
}


void Camera::SetReverseYawRotationEnable(bool _enable)
{
	reverseYawRotationEnable = _enable;
}


bool Camera::IsReverseYawRotationEnabled()const
{
	return reverseYawRotationEnable;
}


void Camera::SetReversePitchRotationEnable(bool _enable)
{
	reversePitchRotationEnable = _enable;
}


bool Camera::IsReversePitchRotationEnabled()const
{
	return reversePitchRotationEnable;
}



void Camera::SetRotationMode(Camera::RotationMode _rotationMode)
{
	rotationMode = _rotationMode;   

	if (rotationMode == RotationMode::OBJECT_VIEWER)     
	{
		LookAt(viewingPoint + rotationMatrix[glm::Z] * distanceToViewingPoint,                        
			   viewingPoint,     
			   viewingPointYAxis);  
	}
	else if (rotationMode == RotationMode::FIRST_PERSON)       
	{
		if (movementType == MovementMode::OVERLAND || movementType == MovementMode::SPECTATOR)       
		{
			ResetRoll();  
		}
	}
}


Camera::RotationMode Camera::GetRotationMode()const
{
	return rotationMode;
}



void Camera::SetViewingPoint(glm::vec3 _point)
{
	viewingPoint = _point;

	if (rotationMode == RotationMode::OBJECT_VIEWER)
	{
		UpdateViewMatrix();
	}
}


glm::vec3 Camera::GetViewingPoint()const
{
	return viewingPoint;
}


void Camera::SetViewingPointYAxis(glm::vec3 _axis)
{
	viewingPointYAxis = _axis;

	if (rotationMode == RotationMode::OBJECT_VIEWER)
	{
		UpdateViewMatrix();    
	}
}


glm::vec3 Camera::GetViewingPointYAxis()const
{
	return viewingPointYAxis;
}


void Camera::UpdateTrackballObjectViewer(float _pitch, float _yaw, float _roll)
{
	if (rotationEnable)
	{
		if (pitchRotationEnable && reversePitchRotationEnable) { _pitch *= -1; }    
		if (yawRotationEnable && reverseYawRotationEnable) { _yaw *= -1; }    

		if (pitchRotationEnable)
		{
			AccumPitch(_pitch);      
			PitchClamp(_pitch);      
		}

		if (yawRotationEnable)
		{
			AccumYaw(_yaw);   
			YawClamp(_yaw);      
		}
			
		rotation = glm::normalize(glm::quat(glm::vec3(pitchRotationEnable ? pitch : 0.0f,
				                                      yawRotationEnable ? yaw : 0.0f,
				                                      0.0f)));
		UpdateViewMatrix();
	}
}


void Camera::UpdateTrackballFirstPersonWithSpectator(float _pitch, float _yaw, float _roll)
{
	if (rotationEnable)
	{ 
		if (pitchRotationEnable && reversePitchRotationEnable) { _pitch *= -1; }    
		if (yawRotationEnable && reverseYawRotationEnable) { _yaw *= -1; }    
		 
		if (pitchRotationEnable)
		{
			AccumPitch(_pitch);      
			PitchClamp(_pitch);      
		}

		if (yawRotationEnable)
		{
			AccumYaw(_yaw);   
			YawClamp(_yaw);      
		}

		rotation = glm::normalize(glm::quat(glm::vec3(pitchRotationEnable ? pitch : 0.0f, 
			                                          yawRotationEnable ? yaw : 0.0f, 
			                                          0.0f)));
		UpdateViewMatrix();
	} 
}



void Camera::UpdateTrackballFirstPersonWithOverland(float _pitch, float _yaw, float _roll)
{
	if (rotationEnable)
	{
		if (pitchRotationEnable && reversePitchRotationEnable) { _pitch *= -1; }    
		if (yawRotationEnable && reverseYawRotationEnable) { _yaw *= -1; }    

		if (pitchRotationEnable)
		{
			AccumPitch(_pitch);      
			PitchClamp(_pitch);      
		}

		if (yawRotationEnable)
		{
			AccumYaw(_yaw);   
			YawClamp(_yaw);      
		}

		rotation = glm::normalize(glm::quat(glm::vec3(pitchRotationEnable ? pitch : 0.0f,
			                                          yawRotationEnable ? yaw : 0.0f,
			                                          0.0f)));
		UpdateViewMatrix();
	}
}


 
void Camera::UpdateTrackballFirstPersonWithFlight(float _pitch, float _yaw, float _roll)
{
	if (rotationEnable)
	{
		if (pitchRotationEnable && reversePitchRotationEnable) { _pitch *= -1; }    
		if (yawRotationEnable && reverseYawRotationEnable) { _yaw *= -1; }    

		if (pitchRotationEnable) { AccumPitch(_pitch); }   
		if (yawRotationEnable) { AccumYaw(_yaw); }   
		if (rollRotationEnable) { AccumRoll(_roll); }   

		if (_pitch != 0.0f || _yaw != 0.0f || _roll != 0.0f)        
		{
			rotation = glm::normalize(glm::quat(glm::vec3(pitchRotationEnable ? pitch : 0.0f,
				                                          yawRotationEnable ? yaw : 0.0f,
				                                          rollRotationEnable ? roll : 0.0f)));
			UpdateViewMatrix();
		}
	}
}


void Camera::ReverseLook()
{
	rotationMatrix[glm::Z] = -rotationMatrix[glm::Z];
	rotationMatrix[glm::X] = -rotationMatrix[glm::X]; 
	roll = -roll;
	pitch = -pitch;

	rotation = glm::quat_cast(rotationMatrix);    
	 
	UpdateViewMatrix();
}


bool Camera::IsRotationEnabled()const
{
	return rotationEnable;
}


void Camera::SetPitchClampEnable(bool _enable)
{
	pitchClampEnable = _enable;
}


bool Camera::IsPitchClampEnabled()const
{
	return pitchClampEnable;
}

  
void Camera::SetPitchMin(float _radians)
{
	pitchMin = _radians;

	ClampRadians180(pitchMin);
}



void Camera::SetPitchMax(float _radians)
{
	pitchMax = _radians;

	ClampRadians180(pitchMax);
}



void Camera::PitchClamp(float& _radians)
{
	if (pitchClampEnable)    
	{
		if (pitch < pitchMin)     
		{
			_radians = pitchMin - (pitch - _radians);
			pitch = pitchMin;
		}
		else if (pitch > pitchMax)     
		{
			_radians = pitchMax - (pitch - _radians);
			pitch = pitchMax;
		}
	}
}



float Camera::GetPitchMin()const
{
	return pitchMin;
}

 
float Camera::GetPitchMax()const
{
	return pitchMax;
}


void Camera::RollClamp(float& _radians)
{
	if (rollClampEnable)    
	{
		if (roll < rollMin)     
		{
			_radians = rollMin - (roll - _radians);
			roll = rollMin;
		}
		else if (roll > rollMax)     
		{
			_radians = rollMax - (roll - _radians);
			roll = rollMax;
		}
	}
}



void Camera::SetRollMin(float _radians)
{
	rollMin = _radians;

	ClampRadians180(rollMin);
}




void Camera::SetRollMax(float _radians)
{
	rollMax = _radians;

	ClampRadians180(rollMax);
}

 
float Camera::GetRollMin()const
{
	return rollMax;
}


float Camera::GetRollMax()const
{
	return rollMax;
}



bool Camera::IsRollClampEnabled()const
{
	return rollClampEnable;
}


void Camera::SetRollClampEnable(bool _enable)
{
	rollClampEnable = _enable;
}


void Camera::SetYawClampEnable(bool _enable)
{
	yawClampEnable = _enable;
}

	
bool Camera::IsYawClampEnabled()const
{
	return yawClampEnable;
}


void Camera::SetPitchRotationEnable(bool _enable)
{
	pitchRotationEnable = _enable;
}


bool Camera::IsPitchRotationEnabled()const
{
	return pitchRotationEnable;
}


glm::vec3 Camera::GetLook()const
{
	return look;
}


 
void Camera::MouseMoveEventHandler(float _xPosOffset, float _yPosOffset)
{
	if (rotationEnable)   
	{
		if (rotationMode == RotationMode::FIRST_PERSON)       
		{ 
			if (movementType == MovementMode::OVERLAND)     
			{
				UpdateTrackballFirstPersonWithOverland(glm::radians(_yPosOffset * horizontalSensitivity),      
					                                   glm::radians(_xPosOffset * verticalSensitivity),      
					                                   0.0f);  
			}
			if (movementType == MovementMode::SPECTATOR)     
			{
				UpdateTrackballFirstPersonWithSpectator(glm::radians(_yPosOffset * horizontalSensitivity),      
					                                    glm::radians(_xPosOffset * verticalSensitivity),      
					                                    0.0f);  
			}
			else if (movementType == MovementMode::FLIGHT)     
			{
				UpdateTrackballFirstPersonWithFlight(glm::radians(_yPosOffset * horizontalSensitivity),      
					                                 glm::radians(-look.x * 10.0f * (1.0f / 60.0f)),              
					                                 glm::radians(_xPosOffset * verticalSensitivity));      						  
			}
		}
		else if (rotationMode == RotationMode::OBJECT_VIEWER)       
		{
			UpdateTrackballObjectViewer(-glm::radians(_yPosOffset * horizontalSensitivity),      
				                        -glm::radians(_xPosOffset * verticalSensitivity),      
				                        0.0f);   
		}
	}
}


void Camera::ResetRoll()
{
	if (rotationMode == RotationMode::FIRST_PERSON)       
	{
		LookAt(GetPosition(), GetPosition() + look, glm::init<glm::vec3>::unitY());
	}
	else if (rotationMode == RotationMode::OBJECT_VIEWER)     
	{
		LookAt(GetPosition(), viewingPoint, glm::init<glm::vec3>::unitY());
	}
}

void Camera::AccumPitch(float _radians)
{
	if (_radians != 0.0f)      
	{
		pitch += _radians;      

		ClampRadians180(pitch);       
	}
}

void Camera::AccumYaw(float _radians)
{
	if (_radians != 0.0f)      
	{
		yaw += _radians;      

		ClampRadians180(yaw);       
	}
}


void Camera::AccumRoll(float _radians)
{
	if (_radians != 0.0f)      
	{
		roll += _radians;      

		ClampRadians180(roll);       
	}
}






void Camera::Walk(float _units)
{
	if (_units != 0.0f)
	{
		if (movementType == MovementMode::OVERLAND)          
		{
			SetPosition(GetPosition() + glm::vec3(look.x, 0.0f, look.z) * _units);                         
		}
		else if (movementType == MovementMode::SPECTATOR)       
		{
			SetPosition(GetPosition() + look * _units);          
		}
		else if (movementType == MovementMode::FLIGHT)       
		{
			SetPosition(GetPosition() + look * _units);          
		}
	}
}



void Camera::Strafe(float _units)
{
	if (_units != 0.0f)
	{
		if (movementType == MovementMode::OVERLAND)           
		{
			SetPosition(GetPosition() + glm::vec3(rotationMatrix[glm::X][glm::X], 0.0f, rotationMatrix[glm::X][glm::Z]) * _units);              
		}
		else if (movementType == MovementMode::SPECTATOR)       
		{
			SetPosition(GetPosition() + rotationMatrix[glm::X] * _units);         
		}
		if (movementType == MovementMode::FLIGHT)       
		{
			SetPosition(GetPosition() += rotationMatrix[glm::X] * _units);         
		}
	}
}



void Camera::Fly(float _units)
{
	if (_units != 0.0f)
	{
		if (movementType == MovementMode::FLIGHT)       
		{
			SetPosition(GetPosition() + rotationMatrix[glm::Y] * _units);         
		}
		else if (movementType == MovementMode::SPECTATOR)       
		{
			SetPosition(GetPosition() + rotationMatrix[glm::Y] * _units);         
		}
	}
}



void Camera::DisplacePosition(glm::vec3 _offset)
{
	SetPosition(position + _offset);
}


void Camera::SetPosition(glm::vec3 _position)
{
	if (rotationMode == RotationMode::OBJECT_VIEWER)
	{
		viewingPoint = _position;
	}
	else     
	{
		position = _position;
	}

	UpdateViewMatrixTranslation();    
}



void Camera::MoveTo(glm::vec3 _direction, float _units)
{
	if (_units != 0.0f) { SetPosition(position + _direction * _units); }      
}




float Camera::GetZoomNear()const
{
	return zoomNear;
}



void Camera::SetZoomNear(float _zoomNear)
{
	zoomNear = _zoomNear;

	SetDistanceToViewingPoint(distanceToViewingPoint);
}



float Camera::GetFov()const
{
	return fov;
}


Camera::MovementMode Camera::GetMovementMode()const
{
	return movementType;
}


void Camera::SetMovementMode(Camera::MovementMode _movementType)
{
	movementType = _movementType;   

	if (rotationMode == RotationMode::FIRST_PERSON)       
	{
		if (movementType == MovementMode::OVERLAND || movementType == MovementMode::SPECTATOR)       
		{
			ResetRoll();  
		}
	}
}


void Camera::SetFov(float _fov)
{
	if (_fov > glm::pi<float>() || _fov < 0.0f) { return; }           

	fov = _fov;   

	UpdateProjectionMatrix();
}


float Camera::GetZoomSpeed()const
{
	return zoomSpeed;
}


void Camera::SetZoomSpeed(float _zoomSpeed)
{
	zoomSpeed = _zoomSpeed;
}


void Camera::SetZoomFar(float _zoomFar)
{
	zoomFar = _zoomFar;

	SetDistanceToViewingPoint(distanceToViewingPoint);
}



float Camera::GetZoomFar()const
{
	return zoomFar;
}


glm::vec3 Camera::GetPosition()const
{
	return position;
}


void Camera::SetMovementSpeed(float _speed)
{
	movementSpeed = _speed;
}


float Camera::GetMovementSpeed()const
{
	return movementSpeed;
}

void Camera::Move(Camera::Movement _direction, float _deltaTime)
{
	float velocity = movementSpeed * _deltaTime;

	switch (_direction)
	{
		case FORWARD:
		{
			Walk(velocity);
			break;
		}

		case BACKWARD:
		{
			Walk(-velocity);
			break;
		}

		case LEFT:
		{
			Strafe(-velocity);
			break;
		}

		case RIGHT:
		{
			Strafe(velocity);
			break;
		}

		case UP:
		{
			Fly(velocity);
			break;
		}

		case DOWN:
		{
			Fly(-velocity);
			break;
		}
	}

	UpdateViewMatrixTranslation();   
}



void Camera::UpdateProjectionMatrix()
{
	projectionMatrix = glm::perspective(fov,           
		                                viewportRatio,    
		                                zNear,    
		                                zFar);   
}


void Camera::UpdateViewMatrixTranslation()
{
	if (rotationMode == RotationMode::OBJECT_VIEWER)
	{
		position = viewingPoint + rotationMatrix[glm::Z] * distanceToViewingPoint;            
	}

	viewMatrix[glm::W][glm::X] = -glm::dot(rotationMatrix[glm::X], GetPosition());
	viewMatrix[glm::W][glm::Y] = -glm::dot(rotationMatrix[glm::Y], GetPosition());
	viewMatrix[glm::W][glm::Z] = -glm::dot(rotationMatrix[glm::Z], GetPosition());
}



void Camera::UpdateViewMatrix()
{ 
	rotationMatrix = glm::mat3_cast(GetRotation());                 
	 
	look = -rotationMatrix[glm::Z];   
	
	viewMatrix[glm::X][glm::X] = rotationMatrix[glm::X][glm::X];   viewMatrix[glm::X][glm::Y] = rotationMatrix[glm::Y][glm::X];   viewMatrix[glm::X][glm::Z] = rotationMatrix[glm::Z][glm::X];
	viewMatrix[glm::Y][glm::X] = rotationMatrix[glm::X][glm::Y];   viewMatrix[glm::Y][glm::Y] = rotationMatrix[glm::Y][glm::Y];   viewMatrix[glm::Y][glm::Z] = rotationMatrix[glm::Z][glm::Y];
	viewMatrix[glm::Z][glm::X] = rotationMatrix[glm::X][glm::Z];   viewMatrix[glm::Z][glm::Y] = rotationMatrix[glm::Y][glm::Z];   viewMatrix[glm::Z][glm::Z] = rotationMatrix[glm::Z][glm::Z];

	if (rotationMode == RotationMode::OBJECT_VIEWER)
	{
		position = viewingPoint + rotationMatrix[glm::Z] * distanceToViewingPoint;            
	}

	viewMatrix[glm::W][glm::X] = -glm::dot(rotationMatrix[glm::X], GetPosition());
	viewMatrix[glm::W][glm::Y] = -glm::dot(rotationMatrix[glm::Y], GetPosition());
	viewMatrix[glm::W][glm::Z] = -glm::dot(rotationMatrix[glm::Z], GetPosition());

}


float Camera::GetDistanceToViewingPoint()const
{
	return distanceToViewingPoint;
}


void Camera::SetDistanceToViewingPoint(float _distance)
{
	distanceToViewingPoint = std::fmin(std::fmax(_distance, zoomNear), zoomFar);      

	if (rotationMode == RotationMode::OBJECT_VIEWER)
	{
		UpdateViewMatrix();    
	}
}


void Camera::Zoom(float _zoom)
{
	if (zoomEnable)   
	{
		if (rotationMode == RotationMode::OBJECT_VIEWER)
		{
			distanceToViewingPoint += -_zoom * zoomSpeed;      
			distanceToViewingPoint = std::fmin(std::fmax(distanceToViewingPoint, zoomNear), zoomFar);      
			UpdateViewMatrix();   
		}
		else if (rotationMode == RotationMode::FIRST_PERSON)
		{
			float newFov = fov + glm::radians(-_zoom * zoomSpeed);
			newFov = std::fmin(std::fmax(newFov, zoomNear), zoomFar);       
			fov = newFov;   
			UpdateProjectionMatrix();   
		}
	}
}
