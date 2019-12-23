#pragma once
#include <string>

namespace Rendy
{
	namespace ES3
	{
		const std::string default_shader_executor_vertex =
			R"(
			#ifdef USE_VERTEX_POSITION
				in vec3 a_position;
				out vec3 v_position;
			#endif

			#ifdef USE_VERTEX_COORD
				in vec2 a_coord;
				out vec2 v_coord;
			#endif

			#ifdef USE_VERTEX_COLOR
				in vec4 a_color;
				out vec4 v_color; 
			#endif

			#ifdef USE_VERTEX_NORMAL
				in vec3 a_normal;
				out vec3 v_normal;
			#endif

			#ifdef USE_VERTEX_TANGENT
				in vec3 a_tangent;
			#endif

			#ifdef USE_VERTEX_BITANGENT 
				in vec3 a_bitangent;
			#endif

			#ifdef USE_VERTEX_NORMAL
				#ifdef USE_VERTEX_TANGENT
					#ifdef USE_VERTEX_BITANGENT
						#define USE_VERTEX_TBN_MATRIX
					#endif
				#endif
			#endif

			#ifdef USE_VERTEX_TBN_MATRIX
				out mat3 v_tbn;
			#endif

			#ifdef USE_VERTEX_BONES
				in vec4 a_bone_id;
				in vec4 a_weight;
				out vec4 v_weight;

				uniform mat4 u_bones[MAX_BONES];
			#endif
  
			uniform mat4 u_transform;
			uniform mat4 u_view;
			uniform mat4 u_projection;
			uniform mat4 u_view_projection;

			void main()
			{
				mat4 bone_transform = mat4(1.0);

			#ifdef USE_VERTEX_BONES
				bone_transform = u_bones[int(a_bone_id[0])] * a_weight[0];
				bone_transform += u_bones[int(a_bone_id[1])] * a_weight[1];
				bone_transform += u_bones[int(a_bone_id[2])] * a_weight[2];
				bone_transform += u_bones[int(a_bone_id[3])] * a_weight[3];
			#endif

			#ifdef USE_VERTEX_BONES
				v_weight = a_weight;
			#endif

			#ifdef USE_VERTEX_POSITION
				gl_Position = u_view_projection * u_transform * 
					bone_transform * vec4(a_position, 1.0);
				v_position = vec3(u_transform * bone_transform * 
					vec4(a_position, 1.0));
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
				v_normal = normalize(vec3(u_transform * bone_transform *
					vec4(a_normal, 0.0)));
			#endif

			#ifdef USE_VERTEX_TANGENT
				vec3 v_tangent = normalize(vec3(u_transform * bone_transform * 
					vec4(a_tangent, 0.0)));
			#endif

			#ifdef USE_VERTEX_BITANGENT
				vec3 v_bitangent = normalize(vec3(u_transform * bone_transform * 
					vec4(a_bitangent, 0.0)));
			#endif

			#ifdef USE_VERTEX_TBN_MATRIX
				v_tbn = mat3(v_tangent, v_bitangent, v_normal);
			#endif
			}
		)";

		const std::string default_shader_executor_fragment =
			R"(
			out vec4 output_color;

			vec3 tonemap_ACES(vec3 color)
			{
				const float A = 2.51;
				const float B = 0.03;
				const float C = 2.43;
				const float D = 0.59;
				const float E = 0.14;
				const float gamma = 2.2;

				return pow(clamp((color * (A * color + B)) / (color * (C * color + D) + E), 0.0, 1.0),
					vec3(1.0/gamma));
			}

			void main()
			{
				vec4 result = shade();
				result.rgb = tonemap_ACES(result.rgb);
				output_color = result;
			} 
		)";
	};
};