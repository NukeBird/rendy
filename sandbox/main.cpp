#include <SDL.h>
#include <engine/rendy.h>
//#include "rendy.h"
#include <regex>
#include <chrono>
#include <optick.h>
#include <engine/util/log.h>
#include <engine/shader/default_shader_executor_es3.hpp>
#include <engine/shader/shader_source.h>

void GLAPIENTRY message_callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::string message_string;

	if (type == GL_DEBUG_TYPE_ERROR)
	{
		message_string = "(OpenGL error)" + std::string(message);
		Rendy::Log::error(message_string);
		return;
	}
	else
	{
		message_string = "(OpenGL other)" + std::string(message);
	}

	Rendy::Log::info(message_string);
}

#include <engine/engine.h>
#include <engine/util/vfs/vfs.h>
#include <engine/pipeline/render_pass/self_sufficient_pass.h>
#include <engine/pipeline/render_pass/depth_prepass.h>
#include <engine/pipeline/render_pass/opaque_pass.h>
#include <engine/pipeline/render_pass/transparent_pass.h>

int main(int argc, char** argv) 
{
	Rendy::Log::init_logger();

	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		Rendy::Log::info("Error while initializing SDL: {0}" , SDL_GetError());
		SDL_Quit();
		return -1;
	}

	glClear(GL_COLOR_BUFFER_BIT);

	#ifdef _WIN32
		Rendy::Log::info("SDL_GL_CONTEXT_MAJOR_VERSION {0}",
			!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4));
		Rendy::Log::info("SDL_GL_CONTEXT_MINOR_VERSION {0}",
			!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));
		Rendy::Log::info("SDL_GL_CONTEXT_PROFILE_MASK {0}",
			!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				SDL_GL_CONTEXT_PROFILE_COMPATIBILITY));
		Rendy::Log::info("SDL_GL_RED_SIZE {0}",
			!SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8));
		Rendy::Log::info("SDL_GL_GREEN_SIZE {0}",
			!SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8));
		Rendy::Log::info("SDL_GL_BLUE_SIZE {0}",
			!SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8));
		Rendy::Log::info("SDL_GL_DEPTH_SIZE {0}",
			!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));
		Rendy::Log::info("SDL_GL_DOUBLEBUFFER {0}",
			!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
		Rendy::Log::info("SDL_GL_ACCELERATED_VISUAL {0}",
			!SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1));
		Rendy::Log::info("SDL_GL_MULTISAMPLEBUFFERS {0}",
			!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1));
		Rendy::Log::info("SDL_GL_MULTISAMPLESAMPLES {0}",
			!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4));
		Rendy::Log::info("SDL_GL_CONTEXT_FLAGS {0}",
			!SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 
				SDL_GL_CONTEXT_DEBUG_FLAG));
		//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#endif

	int width = 1280;
	int height = 720;

	SDL_DisplayMode displayMode;
	SDL_GetDesktopDisplayMode(0, &displayMode);

	//width = static_cast<int>(displayMode.w * 0.8f);
	//height = static_cast<int>(displayMode.h * 0.8f);

	SDL_Window *window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);	

	#ifdef _WIN32
		glewExperimental = GL_TRUE;
		auto init_res = glewInit();
		if (init_res != GLEW_OK)
		{
			Rendy::Log::info(glewGetErrorString(glewInit()));
		}

		/*if (SDL_GL_SetSwapInterval(-1) != 0) //adaptive v-sync
		{
			SDL_GL_SetSwapInterval(0); //enable v-sync
		}*/
		SDL_GL_SetSwapInterval(0); //disable v-sync

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(message_callback, 0);
		//glBindTexture(-24, -3);
		glDisable(GL_POLYGON_SMOOTH);
	#endif // _WIN32

	Rendy::VFSRef vfs = std::make_shared<Rendy::VFS>();
	Rendy::EngineRef engine = std::make_shared<Rendy::Engine>(Rendy::OGL::ES31, vfs);

	auto shader_executor = std::make_shared<Rendy::ShaderSource>(
		Rendy::ES3::default_shader_executor_vertex,
		Rendy::ES3::default_shader_executor_fragment);

	auto pipe = std::make_shared<Rendy::Pipeline>();
	pipe->add_pass(std::make_shared<Rendy::DepthPrepass>(shader_executor, false));
	pipe->add_pass(std::make_shared<Rendy::OpaquePass>(shader_executor, true));
	pipe->add_pass(std::make_shared<Rendy::TransparentPass>(shader_executor));
	engine->set_pipeline(pipe);

	Rendy::ModelFactory model_factory(engine, vfs);

	auto model = model_factory.make("assets/doll.glb");
	Rendy::Log::info("Material count: {0}", model->get_material_count());
	Rendy::Log::info("Node count: {0}", model->get_node_count());
	Rendy::Log::info("Mesh count: {0}", model->get_mesh_count());

	GLuint fucking_vao; //should be active at least one _REAL_ vao
	glGenVertexArrays(1, &fucking_vao);
	glBindVertexArray(fucking_vao); //TODO: bind inside of ES2::Engine if necessary

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	//glEnable(GL_CULL_FACE);

	auto last = std::chrono::steady_clock::now();

	/*std::smatch m;
	std::regex rgx("#ifdef (\\w+)[^ ]*\\n");

	auto s = generic_vertex_shader;

	while (std::regex_search(s, m, rgx)) 
	{
		std::cout << m[1].str() << std::endl;
		s = m.suffix().str();
	}*/

	SDL_Event event;
	bool is_running = true;
	while (is_running)
	{
		OPTICK_FRAME("Frame");

		static float cam_fov(50.0f);
		static float cam_aspect = width / static_cast<float>(height);
		static glm::vec3 cam_pos(0.0, 0.0, 2.8);
		static float cam_radius(1.5f);
		static glm::vec3 cam_target(0.0, 0.0, 1.3);
		static float camera_near = 0.01f;
		static float camera_far = 1000.0f;

		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<float> d = now - last;
		float dt = glm::max(0.0001f, d.count());
		last = now;

		model->update(dt);

		static float x_angle = 0.0f;
		static float y_angle = 0.0f;

		static bool turn_left = false;
		static bool turn_right = false;
		static bool turn_up = false;
		static bool turn_down = false;
		static bool move_up = false;
		static bool move_down = false;
		static bool increase_r = false;
		static bool decrease_r = false;
		static float height = 0.0f;

		OPTICK_PUSH("SDL_PollEvent");
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_running = false;
			}
			
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_LEFT:  
					{
						turn_left = true;
						break;
					}
					case SDLK_RIGHT:
					{
						turn_right = true;
						break;
					}
					case SDLK_UP:
					{
						turn_up = true;
						break;
					}
					case SDLK_DOWN:
					{
						turn_down = true;
						break;
					}
					case SDLK_w:
					{
						decrease_r = true;
						break;
					}
					case SDLK_s:
					{
						increase_r = true;
						break;
					}
					case SDLK_e:
					{
						move_up = true;
						break;
					}
					case SDLK_d:
					{
						move_down = true;
						break;
					}
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_LEFT:
					{
						turn_left = false;
						break;
					}
					case SDLK_RIGHT:
					{
						turn_right = false;
						break;
					}
					case SDLK_UP:
					{
						turn_up = false;
						break;
					}
					case SDLK_DOWN:
					{
						turn_down = false;
						break;
					}
					case SDLK_w:
					{
						decrease_r = false;
						break;
					}
					case SDLK_s:
					{
						increase_r = false;
						break;
					}
					case SDLK_e:
					{
						move_up = false;
						break;
					}
					case SDLK_d:
					{
						move_down = false;
						break;
					}
				}
			}
		}
		OPTICK_POP();

		const float turning_speed = 60.0f;
		const float r_speed = 2.0f;

		if (turn_left)
		{
			x_angle = glm::mod(x_angle - turning_speed * dt, 360.0f);
		}

		if (turn_right)
		{
			x_angle = glm::mod(x_angle + turning_speed * dt, 360.0f);
		}

		if (turn_up)
		{
			y_angle = glm::clamp(y_angle - turning_speed * dt, -80.0f, 80.0f);
		}

		if (turn_down)
		{
			y_angle = glm::clamp(y_angle + turning_speed * dt, -80.0f, 80.0f);
		}

		if (increase_r)
		{
			cam_radius = glm::clamp(cam_radius + dt, 0.000001f, 20.0f);
		}

		if (decrease_r)
		{
			cam_radius = glm::clamp(cam_radius - dt, 0.001f, 20.0f);
		}

		const float height_speed = 0.25f;

		if (move_up)
		{
			height = glm::clamp(height + height_speed * dt, -2.0f, 2.0f);
		}

		if (move_down)
		{
			height = glm::clamp(height - height_speed * dt, -2.0f, 2.0f);
		}

		glm::mat4 view = glm::lookAt(
			glm::vec3(
				glm::rotate(glm::radians(x_angle), glm::vec3(0, 1, 0)) * 
				glm::rotate(glm::radians(y_angle), glm::vec3(1, 0, 0)) *
				glm::vec4(0.0, 0.0, cam_radius, 1.0)) + cam_target, cam_target,
			glm::vec3(0, 1, 0));

		glm::mat4 proj = glm::perspective(glm::radians(cam_fov),
			cam_aspect, camera_near, camera_far);

		glm::mat4 transform = glm::translate(cam_target + glm::vec3(0, height, 0)) *
			//glm::rotate(glm::radians(angle), glm::vec3(0, 1, 0)) *
			glm::scale(glm::vec3{ 0.35f });

		glClearColor(0.015f, 0.015f, 0.015f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//model->draw(transform, view, proj);
		engine->push(model, transform, view, proj);
		engine->flush();

		/*OPTICK_PUSH("glFinish");
		glFinish();
		OPTICK_POP();*/

		OPTICK_PUSH("SwapWindow");
		SDL_GL_SwapWindow(window);
		OPTICK_POP();
	}

	SDL_GL_DeleteContext(glcontext);  
	SDL_DestroyWindow(window);

	//OPTICK_SAVE_CAPTURE("capture.opt");

	return 0;
}