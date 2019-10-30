#include "perspective_camera.h"
#define GLM_ENABLE_EXPERIMENTAL //TODO: to common
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <optick.h>

Rendy::PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio,
	const glm::vec3& position, const glm::vec3& rotation, float near, float far)
{
	OPTICK_EVENT();
	this->fov = fov;
	this->near = near;
	this->far = far;
	this->aspect_ratio = aspect_ratio;
	this->position = position;
	this->rotation = rotation;

	//TODO: validation + warnings
}

bool Rendy::PerspectiveCamera::look_at(const glm::vec3& position)
{
	OPTICK_EVENT();
	return false;
}

void Rendy::PerspectiveCamera::translate(const glm::vec3& translation)
{
	OPTICK_EVENT();
}

void Rendy::PerspectiveCamera::rotate(const glm::vec3& rotation)
{
	OPTICK_EVENT();
}

void Rendy::PerspectiveCamera::set_aspect_ratio(float aspect_ratio)
{
	OPTICK_EVENT();
}

void Rendy::PerspectiveCamera::set_position(const glm::vec3& position)
{
	OPTICK_EVENT();
}

void Rendy::PerspectiveCamera::set_rotation(const glm::vec3& rotation)
{
	OPTICK_EVENT();
}

bool Rendy::PerspectiveCamera::is_dirty() const
{
	OPTICK_EVENT();
	return view_matrix_is_dirty || projection_matrix_is_dirty;
}

const glm::vec3& Rendy::PerspectiveCamera::get_position() const
{
	OPTICK_EVENT();
	return position;
}

const glm::vec3& Rendy::PerspectiveCamera::get_rotation() const
{
	OPTICK_EVENT();
	return rotation;
}

const glm::mat4& Rendy::PerspectiveCamera::get_view_matrix()
{
	OPTICK_EVENT();
	if (view_matrix_is_dirty)
	{
		//TODO
		view_matrix_is_dirty = false;
	}

	return view_matrix;
}

const glm::mat4& Rendy::PerspectiveCamera::get_projection_matrix()
{
	OPTICK_EVENT();
	if (projection_matrix_is_dirty)
	{
		projection_matrix = glm::perspective(glm::radians(fov), 
			aspect_ratio, near, far);
		projection_matrix_is_dirty = false;
	}

	return projection_matrix;
}

const glm::mat4& Rendy::PerspectiveCamera::get_view_projection_matrix()
{
	OPTICK_EVENT();
	if (is_dirty())
	{
		view_projection_matrix = get_projection_matrix() * get_view_matrix();
	}

	return view_projection_matrix;
}
