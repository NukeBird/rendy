#pragma once
#include <string>

namespace Rendy
{
	namespace ES3
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
				out vec3 v_tangent;
			#endif

			#ifdef USE_VERTEX_BITANGENT 
				in vec3 a_bitangent;
				out vec3 v_bitangent;
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
  
			uniform mat4 u_transform;
			uniform mat4 u_view;
			uniform mat4 u_projection;
			uniform mat4 u_view_projection;

			void main()
			{
			#ifdef USE_VERTEX_POSITION
				gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
				v_position = vec3(u_transform * vec4(a_position, 1.0));
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
				v_normal = normalize(vec3(u_transform * vec4(a_normal, 0.0)));
			#endif

			#ifdef USE_VERTEX_TANGENT
				v_tangent = normalize(vec3(u_transform * vec4(a_tangent, 0.0)));
			#endif

			#ifdef USE_VERTEX_BITANGENT
				v_bitangent = normalize(vec3(u_transform * vec4(a_bitangent, 0.0)));
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

			#ifdef USE_METALLIC_ROUGHNESS_TEXTURE
				uniform sampler2D metallic_roughness_texture;
				#define COOK_TORRANCE
			#endif

			#ifndef COOK_TORRANCE
				#define DIFFUSE_RENDERER
			#endif

			uniform samplerCube iem;
			uniform samplerCube pmrem;
			uniform sampler2D lut;
			uniform int u_max_pmrem_level;

			uniform vec3 u_camera_position;

			out vec4 output_color;

			const float gamma = 2.2;
			const float PI = 3.14159265359;

			vec4 read_texture(sampler2D tex, vec2 uv)
			{
				return texture(tex, uv);
			}

			vec4 read_texture(samplerCube tex, vec3 uv)
			{
				return texture(tex, uv);
			}

			vec4 read_texture(samplerCube tex, vec3 uv, float lod)
			{
				return textureLod(tex, uv, lod);
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

			#ifdef COOK_TORRANCE 
				vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
				{
					return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
				}
				
				vec4 calculate_lighting()
				{
					vec3 N = get_normal();
					vec3 V = normalize(u_camera_position - v_position);

					if(dot(N,V) < 0)
					{
						N = -N;
					}

					vec3 R = normalize(reflect(-V, N));
					
					vec4 diffuse = get_diffuse();
					vec3 albedo = diffuse.rgb;
					
					float metallic = read_texture(metallic_roughness_texture, v_coord).b;
					float roughness = read_texture(metallic_roughness_texture, v_coord).g;

					vec3 F0 = vec3(0.04); 
					F0 = mix(F0, albedo, metallic);

					vec3 irradiance = read_texture(iem, N).rgb;
					diffuse = vec4(irradiance, 1.0) * diffuse;

					vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
					vec3 kS = F;
					vec3 kD = 1.0 - F;
					kD *= 1.0 - metallic;
					vec4 ao = vec4(vec3(read_texture(metallic_roughness_texture, v_coord).r), 1.0);

					vec2 envBRDF = read_texture(lut, vec2(roughness, max(dot(N, V), 0.0))).gr;
					vec3 specular = mix(F, albedo, 1.0 - metallic) * textureLod(pmrem, R,  roughness * u_max_pmrem_level).rgb * (envBRDF.x + envBRDF.y); 
					vec4 ambient = (vec4(kD, 1.0) * diffuse + vec4(specular, 0)) * ao;

					return ambient;
				}
			#endif

			#ifdef DIFFUSE_RENDERER
				vec4 calculate_lighting()
				{
					vec4 result = get_diffuse();
					result.rgb *= read_texture(iem, get_normal()).rgb;
					return result;
				}
			#endif

			void main()
			{
				vec4 result = calculate_lighting();

				result.rgb = vec3(1.0) - exp(-result.rgb);
				//result.rgb = result.rgb / (result.rgb + vec3(1.0));
				result.rgb = pow(result.rgb, vec3(1.0/gamma));
				output_color = result;
			} 
		)";
	};
};