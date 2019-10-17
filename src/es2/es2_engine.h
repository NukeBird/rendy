#pragma once
#include "../abstract_engine.h"
#include "../draw_call.h"

class ES2Engine final: public AbstractEngine
{
public:
	virtual void render(const SceneRef& scene, const glm::mat4& model, const glm::mat4& view,
		const glm::mat4& proj) override;
private:
	std::vector<DrawCall> form_draw_calls(const SceneRef& scene, const glm::mat4& model, 
		const glm::mat4& view, const glm::mat4& proj);
	void form_draw_calls(const SceneRef& scene, const NodeS::NodeRef& node,
		const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj,
		std::vector<DrawCall>& draw_calls);
	void render(const DrawCall& draw_call,
		const SceneRef& scene,
		const glm::vec3& camera_position);
};