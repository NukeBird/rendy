#include <iostream>
#include <SDL.h>

#ifdef _WIN32
	#include <gl/glew.h>
#else
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/glplatform.h>
#endif

#include <assimp/pbrmaterial.h>
#include "texture_manager.h"
#include "shader_manager.h"

const char* vtx = 
R"(
	layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
	out vec4 vertexColor; // specify a color output to the fragment shader

	void main()
	{
		gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
		vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
	}
)";

const char* frg =
R"(
	out vec4 FragColor;
  
	in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

	void main()
	{
		FragColor = vertexColor;
		#ifdef USE_VERTEX_COLOR
			FragColor = vec4(1, 1, 1, 1);
		#endif
	} 
)";

#include "es2/vertex_array.h"
#include "model_builder.h"

int main(int argc, char** argv) 
{
	//TextureManager::get_instance();
	//ShaderManager::get_instance();
	/*Assimp::Importer imp;

	unsigned int flags =
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_GenNormals |
		aiProcess_GenSmoothNormals |
		aiProcess_FindInstances |
		aiProcess_CalcTangentSpace |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SplitByBoneCount |
		aiProcess_SortByPType;

	auto scene = imp.ReadFile("bird.fbx", flags);
	//Animator anim(scene, 0);

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		auto& mat = *(scene->mMaterials[i]);

		std::cout << "Texture #" << i << std::endl;
		std::cout << "Name: " << mat.GetName().C_Str() << std::endl;

		aiString path;
		mat.GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &path);
		std::cout << "METALLICROUGHNESS: " << path.length  << "(" << path.C_Str() << ")" << std::endl;

		mat.GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, &path);
		std::cout << "ALBEDO: " << path.length << "(" << path.C_Str() << ")" << std::endl;

		std::cout << std::endl;
	}*/

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
	#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#endif

	SDL_Window *window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);	

	#ifdef _WIN32
		glewExperimental = GL_TRUE;
		auto init_res = glewInit();
		if (init_res != GLEW_OK)
		{
			std::cout << glewGetErrorString(glewInit()) << std::endl;
		}
	#endif // _WIN32

	auto sh = ShaderManager::get_instance()->make(vtx, frg);
	std::cout << "VALIDATION " << sh->validate() << std::endl;

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

	auto layout = std::make_shared<BufferLayout>(std::initializer_list<BufferElement>{{ShaderDataType::Float3, "aPos"} });

	ES2::VertexArray vao(verts, 12 * sizeof(float),
		indices, 6 * sizeof(uint16_t), layout);

	std::cout << "VAO status: " << vao.validate() << std::endl;

	glDisable(GL_CULL_FACE);

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

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto shader = sh->compile(ShaderFlag::USE_VERTEX_COLOR);
		shader->bind();
		vao.bind();
		vao.draw();
		vao.unbind();
		shader->unbind();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glcontext);

	return 0;
}