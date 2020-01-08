#pragma once
#include <string>

namespace Rendy
{
	namespace ES2
	{
		const std::string default_vertex_shader =
		R"(
			precision highp float;

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

		const std::string default_fragment_shader =
		R"(
		precision mediump float;

		#ifdef USE_VERTEX_POSITION
			in vec3 v_position;
		#endif

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

		#ifdef USE_VERTEX_NORMAL
			#ifdef USE_VERTEX_TANGENT
				#ifdef USE_VERTEX_BITANGENT
					#define USE_VERTEX_TBN_MATRIX
				#endif
			#endif
		#endif

		#ifdef USE_VERTEX_TBN_MATRIX
			in mat3 v_tbn;
		#endif

		#ifdef USE_COLOR_TEXTURE
			uniform sampler2D color_texture;
		#endif

		#ifdef USE_NORMAL_TEXTURE
			uniform sampler2D normal_texture;
		#endif

			uniform samplerCube u_iem;

			uniform vec3 u_camera_position;

			out vec4 output_color;

			const float gamma = 2.2;
			const float PI = 3.14159265359;

			vec4 read_texture(sampler2D tex, vec2 uv)
			{
				#if (__VERSION__ >= 300)
					return texture(tex, uv);
				#else	
					return texture2D(tex, uv);
				#endif
			}

			vec4 read_texture(samplerCube tex, vec3 uv)
			{
				#if (__VERSION__ >= 300)
					return texture(tex, uv);
				#else	
					return textureCube(tex, uv);
				#endif
			}

			vec3 get_normal()
			{	
				#ifndef USE_NORMAL_TEXTURE
					#ifdef USE_VERTEX_NORMAL
						return v_normal;
					#endif
				#else
					#ifdef USE_VERTEX_COORD
						#ifndef USE_VERTEX_TBN_MATRIX
							#ifdef USE_VERTEX_NORMAL
								return v_normal; 
							#endif
						#else
							vec3 f_normal = read_texture(normal_texture, v_coord).rgb;
							f_normal = normalize(f_normal * 2.0 - 1.0); 
							f_normal = normalize(v_tbn * f_normal);	
							return f_normal;
						#endif
					#endif
				#endif

				return vec3(1.0);
			}

			vec4 get_diffuse()
			{
				vec4 diffuse = vec4(1.0);
		
				#ifdef USE_VERTEX_COLOR
					diffuse = v_color;
				#endif

				#ifdef USE_VERTEX_COORD
					#ifdef USE_COLOR_TEXTURE
						vec4 diffuse_texture_value = read_texture(color_texture, v_coord);
						diffuse_texture_value.rgb = pow(diffuse_texture_value.rgb, vec3(gamma)); 
						diffuse *= diffuse_texture_value;
						return diffuse;
					#endif
				#endif

				return vec4(0.0);
			}

			void main()
			{
				vec4 result = get_diffuse();
				result.rgb *= read_texture(u_iem, get_normal()).rgb;

				result.rgb = result.rgb / (result.rgb + vec3(1.0));
				result.rgb = pow(result.rgb, vec3(1.0/gamma));
				output_color = result;
			} 
		)";
	};
};