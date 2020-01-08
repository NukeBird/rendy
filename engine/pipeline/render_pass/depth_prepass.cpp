#include "../../common.h"
#include "depth_prepass.h"
#include <algorithm>
#include <optick.h>
#include "../../shader/default_shader_executor_es3.hpp"

Rendy::DepthPrepass::DepthPrepass(ShaderSourceRef executor, bool prepass_transparency)
{
	OPTICK_EVENT();
	this->prepass_transparency = prepass_transparency;
	this->executor = std::make_shared<ShaderSource>(executor->get_source(ShaderType::VertexShader),
		R"(
			out vec4 output_color;
			
			void main()
			{
				output_color = vec4(1, 1, 1, 1);
			}
		)");
}

void Rendy::DepthPrepass::execute(const BatchList& batches)
{
	OPTICK_EVENT();

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glColorMask(0, 0, 0, 0);
	glDepthMask(GL_TRUE);

	for (const auto& batch: batches)
	{
		if (!batch.material->uses_transparency() || prepass_transparency)
		{
			auto command_list = batch.to_command_list(executor);
			for (auto& command : command_list)
			{
				command->execute();
			}
		}
	}
}