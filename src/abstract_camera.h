#pragma once
#include <glm/glm.hpp>
#include <memory>

struct AbstractCamera
{
	virtual bool look_at(const glm::vec3& position) = 0;
	virtual void translate(const glm::vec3& translation) = 0;
	virtual void rotate(const glm::vec3& rotation) = 0;
	virtual void set_aspect_ratio(float aspect_ratio) = 0;
	virtual void set_position(const glm::vec3& position) = 0;
	virtual void set_rotation(const glm::vec3& rotation) = 0;
	virtual bool is_dirty() const = 0;
	virtual const glm::vec3& get_position() const = 0;
	virtual const glm::vec3& get_rotation() const = 0;
	virtual const glm::mat4& get_view_matrix() = 0;
	virtual const glm::mat4& get_projection_matrix() = 0;
	virtual const glm::mat4& get_view_projection_matrix() = 0;
};

using AbstractCameraRef = std::shared_ptr<AbstractCamera>;