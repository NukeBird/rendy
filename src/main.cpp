#include <iostream>
#include <SDL.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

#ifdef _WIN32
	#include <gl/glew.h>
#else
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/glplatform.h>
#endif

#include "texture_manager.h"
#include "shader_manager.h"
#include "es2/vertex_array.h"
#include "model_builder.h"
#include "vertex_array_manager.h"

int main(int argc, char** argv) 
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		std::cout << "Error while initializing SDL: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	#ifdef _WIN32
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#endif

	const int width = 640 * 2;
	const int height = 640;

	SDL_Window *window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);	

	#ifdef _WIN32
		glewExperimental = GL_TRUE;
		auto init_res = glewInit();
		if (init_res != GLEW_OK)
		{
			std::cout << glewGetErrorString(glewInit()) << std::endl;
		}
	#endif // _WIN32

	auto model = ModelBuilder::build("assets/matball.glb");
	std::cout << "Material count: " << model->get_material_count() << std::endl;
	std::cout << "Node count: " << model->get_node_count() << std::endl;
	std::cout << "Mesh count: " << model->get_mesh_count() << std::endl;

	std::cout << "GENERIC SHADER STATUS: "
		<< ShaderManager::get_instance()->get_generic_shader()->validate()
		<< std::endl;

	std::cout << "MODEL STATUS: " << model->validate() << std::endl;

	float verts[] =
	{
		-0.5f,  0.5f,  -1.0f,
		 0.5f,  0.5f,  -1.0f,
		 0.5f, -0.5f,  -1.0f,
		-0.5f, -0.5f,  -1.0f
	};

	uint16_t indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	GLuint fucking_vao; //should be active at least one _REAL_ vao
	glGenVertexArrays(1, &fucking_vao);
	glBindVertexArray(fucking_vao);

	/*auto layout = std::make_shared<BufferLayout>(std::initializer_list<BufferElement>{{ShaderDataType::Float3, "aPos"} });

	auto vao = VertexArrayManager::get_instance()->make(verts, 12 * sizeof(float),
		indices, 6 * sizeof(uint16_t), layout);

	std::cout << "VAO status: " << vao->validate() << std::endl;*/

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	SDL_Event event;
	bool is_running = true;
	while (is_running)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				is_running = false;
			}
		}

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*auto shader = sh->compile(ShaderFlag::USE_VERTEX_COLOR);
		shader->bind();
		vao->bind();
		vao->draw();
		vao->unbind();
		shader->unbind();*/

		static float cam_fov(60.0f);
		static float cam_aspect = width / static_cast<float>(height);
		static glm::vec3 cam_pos(1, 0, 0);
		static glm::vec3 cam_target(0, 0, 0);
		static float near = 0.01f;
		static float far = 100.0f;
		
		glm::mat4 view = glm::lookAt(cam_pos, cam_target,
			glm::vec3(0, 1, 0));

		glm::mat4 proj = glm::perspective(glm::radians(cam_fov), 
			cam_aspect, near, far);

		model->draw(glm::mat4(1.0), view, proj);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glcontext);

	return 0;
}