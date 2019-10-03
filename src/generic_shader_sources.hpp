#pragma once
#include <string>

const std::string generic_vertex_shader =
R"(
#ifdef USE_VERTEX_POSITION
	layout (location = 0) in vec3 a_position;
#endif

#ifdef USE_VERTEX_COORD
	layout (location = 1) in vec2 a_coord;
	out vec2 v_coord;
#endif

#ifdef USE_VERTEX_COLOR
	layout (location = 2) in vec4 a_color;
	out vec4 v_color; 
#endif

#ifdef USE_VERTEX_NORMAL
	layout (location = 3) in vec3 a_normal;
	out vec3 v_normal;
#endif

#ifdef USE_VERTEX_TANGENT
	layout (location = 4) in vec3 a_tangent;
	out vec3 v_tangent;
#endif

#ifdef USE_VERTEX_BITANGENT 
	layout (location = 5) in vec3 a_bitangent;
	out vec3 v_bitangent;
#endif
  
	uniform mat4 u_transform;

	void main()
	{
	#ifdef USE_VERTEX_POSITION
		gl_Position = u_transform * vec4(a_position, 1.0);
	#else
		gl_Position = vec4(vec3(0.0, 0.0, 0.0), 1.0);
	#endif

	#ifdef USE_VERTEX_COORD
		v_coord = a_coord;
	#endif

	#ifdef USE_VERTEX_COLOR
		v_color = a_color;
	#endif

	#ifdef USE_VERTEX_NORMAL
		v_normal = a_normal; //TODO
	#endif

	#ifdef USE_VERTEX_TANGENT
		v_tangent = a_tangent; //TODO
	#endif

	#ifdef USE_VERTEX_BITANGENT
		v_bitangent = a_bitangent; //TODO
	#endif
	}
)";

const std::string generic_fragment_shader =
R"(
#ifdef USE_VERTEX_COORD
	in vec2 v_coord;
#endif

#ifdef USE_VERTEX_COLOR
	in vec4 v_color; 
#endif

#ifdef USE_VERTEX_NORMAL
	in vec3 v_normal;
#endif

#ifdef USE_VERTEX_TANGENT
	in vec3 v_tangent;
#endif

#ifdef USE_VERTEX_BITANGENT 
	in vec3 v_bitangent;
#endif

	out vec4 output_color;

	void main()
	{
		/*#ifdef USE_VERTEX_COLOR
			output_color = v_color;
		#else*/
			output_color = vec4(1, 1, 1, 1);
		//#endif

	#ifdef USE_VERTEX_NORMAL
		output_color = vec4(v_normal*vec3(0.5) + vec3(0.5), 1);
	#endif
	} 
)";