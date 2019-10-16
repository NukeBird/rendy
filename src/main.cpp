#include <iostream>
#include <SDL.h>
#define GLM_FORCE_RADIANS 
#define GLM_ENABLE_EXPERIMENTAL
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE //TODO: ???
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <chrono>
#include <thread>

#ifdef _WIN32
	#include <gl/glew.h>
#else
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/glplatform.h>
#endif

#include "texture_factory.h"
#include "shader_manager.h"
#include "es2/vertex_array.h"
#include "model_builder.h"
#include "vertex_array_factory.h"
#include "vertex_buffer_factory.h"
#include "index_buffer_factory.h"
#include "scene.h"
#include "scene_builder.h"
#include "generic_shader_sources.hpp"
#include <regex>

int main(int argc, char** argv) 
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
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
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#endif

	const int width = 1280;// *2;
	const int height = 720;

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

		SDL_GL_SetSwapInterval(0);
	#endif // _WIN32

	SceneBuilder::build("assets/shoes.glb");

	auto model = ModelBuilder::build("assets/shoes.glb");
	std::cout << "Material count: " << model->get_material_count() << std::endl;
	std::cout << "Node count: " << model->get_node_count() << std::endl;
	std::cout << "Mesh count: " << model->get_mesh_count() << std::endl;

	std::cout << "GENERIC SHADER STATUS: "
		<< ShaderManager::get_instance()->get_generic_shader()->validate()
		<< std::endl;

	//ShaderManager::get_instance()->reload();
	//VertexArrayManager::get_instance()->reload();

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

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto last = std::chrono::steady_clock::now();
	auto last_render = last;

	std::smatch m;
	std::regex rgx("#ifdef (\\w+)[^ ]*\\n");

	auto s = generic_vertex_shader;

	while (std::regex_search(s, m, rgx)) 
	{
		std::cout << m[1].str() << std::endl;
		s = m.suffix().str();
	}

	SDL_Event event;
	bool is_running = true;
	while (is_running)
	{
		ShaderManager::get_instance()->flush();

		static float cam_fov(40.0f);
		static float cam_aspect = width / static_cast<float>(height);
		static glm::vec3 cam_pos(0.0, 0.0, 2.6);
		static glm::vec3 cam_target(0.0, 0.0, 1.3);
		static float near = 0.1f;
		static float far = 20.0f;

		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<float> d = now - last;
		float dt = glm::max(0.0001f, d.count());
		last = now;

		glm::mat4 view = glm::lookAt(cam_pos, cam_target,
			glm::vec3(0, 1, 0));

		glm::mat4 proj = glm::perspective(glm::radians(cam_fov),
			cam_aspect, near, far);

		static float angle = 0.0f;

		angle = glm::mod(angle + 20.0f*dt, 360.0f);

		glm::mat4 transform = glm::translate(cam_target) * 
			glm::rotate(glm::radians(angle), glm::vec3(0, 1, 0)) *
			glm::scale(glm::vec3{ 0.4f });

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_running = false;
			}
		}

		if (std::chrono::duration<float>(now - last_render).count() >= 1.0f / 60.0f)
		{
			last_render = now;
			glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			model->draw(transform, view, proj);
		}
		SDL_GL_SwapWindow(window);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	SDL_GL_DeleteContext(glcontext);

	return 0;
}