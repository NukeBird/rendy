#include "perspective_camera.h"
#define GLM_ENABLE_EXPERIMENTAL //TODO: to common
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio, 
	const glm::vec3& position, const glm::vec3& rotation, float near, float far)
{
	this->fov = fov;
	this->near = near;
	this->far = far;
	this->aspect_ratio = aspect_ratio;
	this->position = position;
	this->rotation = rotation;

	//TODO: validation + warnings
}

bool PerspectiveCamera::look_at(const glm::vec3& position)
{
	return false;
}

void PerspectiveCamera::translate(const glm::vec3& translation)
{
}

void PerspectiveCamera::rotate(const glm::vec3& rotation)
{
}

void PerspectiveCamera::set_aspect_ratio(float aspect_ratio)
{
}

void PerspectiveCamera::set_position(const glm::vec3& position)
{
}

void PerspectiveCamera::set_rotation(const glm::vec3& rotation)
{
}

bool PerspectiveCamera::is_dirty() const
{
	return view_matrix_is_dirty || projection_matrix_is_dirty;
}

const glm::vec3& PerspectiveCamera::get_position() const
{
	// TODO: insert return statement here
}

const glm::vec3& PerspectiveCamera::get_rotation() const
{
	// TODO: insert return statement here
}

const glm::mat4& PerspectiveCamera::get_view_matrix()
{
	if (view_matrix_is_dirty)
	{
		view_matrix_is_dirty = false;
	}
}

const glm::mat4& PerspectiveCamera::get_projection_matrix()
{
	if (projection_matrix_is_dirty)
	{
		projection_matrix = glm::perspective(glm::radians(fov), 
			aspect_ratio, near, far);
		projection_matrix_is_dirty = false;
	}

	return projection_matrix;
}

const glm::mat4& PerspectiveCamera::get_view_projection_matrix()
{
	if (is_dirty)
	{
		view_projection_matrix = get_projection_matrix() * get_view_matrix();
	}

	return view_projection_matrix;
}
