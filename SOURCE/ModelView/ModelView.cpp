#include <ModelView/ModelView.h>


ModelView::ModelView()
{}


void ModelView::SetPosition(glm::vec3 _position)
{
	position = _position;
	transformationValid = false;     
}


glm::mat3 ModelView::GetNormalMatrix()const
{
	return normalMatrix;
}


glm::mat4 ModelView::GetTransformation()
{
	if (!transformationValid)     
	{
		transformation = glm::translate(glm::identity<glm::mat4>(), position) *
			                            glm::mat4_cast(rotation) *
			                            glm::scale(glm::identity<glm::mat4>(), scale);
		
		normalMatrix = glm::transpose(glm::inverse(glm::mat3(transformation)));                                                       
		transformationValid = true;     
	}
	return transformation;
}


void ModelView::SetTransformation(glm::vec3 _scale, glm::quat _rotation, glm::vec3 _position)
{
	scale = _scale;
	rotation = _rotation;
	position = _position;
	transformationValid = false;     
}




glm::vec3 ModelView::GetPosition()const
{
	return position;
}



void ModelView::SetRotation(glm::quat _rotation)
{
	rotation = _rotation;
	transformationValid = false;     
}


glm::quat ModelView::GetRotation()const
{
	return rotation;
}


void ModelView::SetScale(glm::vec3 _scale)
{
	scale = _scale;
	transformationValid = false;     
}



glm::vec3 ModelView::GetScale()const
{
	return scale;
}