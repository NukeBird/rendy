#pragma once
#include "abstract_camera.h"

namespace Rendy
{
	class PerspectiveCamera : public AbstractCamera //TODO
	{
	public:
		PerspectiveCamera(float fov, float aspect_ratio,
			const glm::vec3& position = glm::vec3(0.0, 0.0, 0.0),
			const glm::vec3& rotation = glm::vec3(0.0, 0.0, 0.0),
			float near = 0.1f, float far = 1000.0f);
		virtual bool look_at(const glm::vec3& position) override;
		virtual void translate(const glm::vec3& translation) override;
		virtual void rotate(const glm::vec3& rotation) override;
		virtual void set_aspect_ratio(float aspect_ratio) override;
		virtual void set_position(const glm::vec3& position) override;
		virtual void set_rotation(const glm::vec3& rotation) override;
		virtual bool is_dirty() const override;
		virtual const glm::vec3& get_position() const override;
		virtual const glm::vec3& get_rotation() const override;
		virtual const glm::mat4& get_view_matrix() override;
		virtual const glm::mat4& get_projection_matrix() override;
		virtual const glm::mat4& get_view_projection_matrix() override;
	private:
		bool view_matrix_is_dirty = true;
		bool projection_matrix_is_dirty = true;
		glm::vec3 position;
		glm::vec3 rotation;
		float fov;
		float near;
		float far; //90 - extra wide, 30 - zoomed in
		float aspect_ratio;

		glm::mat4 view_matrix;
		glm::mat4 projection_matrix;
		glm::mat4 view_projection_matrix;
	};
};