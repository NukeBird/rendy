#pragma once
#include <string>

const std::string generic_vertex_shader =
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
	#else
		gl_Position = vec4(vec3(0.0, 0.0, 0.0), 1.0);
	#endif

	v_position = vec3(u_transform * vec4(a_position, 1.0));

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

const std::string generic_fragment_shader =
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
#endif

#ifdef USE_AMBIENT_TEXTURE
	uniform sampler2D ambient_texture;
#endif

#ifdef USE_METALLIC_ROUGHNESS_TEXTURE
	#define MODEL_COOK_TORRANCE
#endif

#ifdef USE_DIRECT_LIGHTS
	uniform vec3 direct_lights[DIRECT_LIGHT_COUNT];
#endif

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

	vec3 get_normal()
	{	
		#ifndef USE_NORMAL_TEXTURE
			return v_normal;
		#else
			#ifdef USE_VERTEX_COORD
				#ifndef USE_VERTEX_TBN_MATRIX
					return v_normal; 
				#else
					vec3 f_normal = read_texture(normal_texture, v_coord).rgb;
					//f_normal *= vec3(vec2(0.85), 1.0); //TODO
					//f_normal.g = 1.0 - f_normal.g;
					f_normal = normalize(f_normal * 2.0 - 1.0); 
					f_normal = normalize(v_tbn * f_normal);	
					return f_normal;
				#endif
			#endif
		#endif

		return v_normal;
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
			#endif
		#endif

		return diffuse;
	}

	vec3 get_ao()
	{
		vec3 ao = vec3(1.0);
		
		#ifdef USE_VERTEX_COORD
			#ifdef USE_AMBIENT_TEXTURE
				ao = vec3(read_texture(ambient_texture, v_coord).r);
			#else
				#ifdef USE_METALLIC_ROUGHNESS_TEXTURE
					ao = vec3(read_texture(metallic_roughness_texture, v_coord).r);
				#endif
			#endif
		#endif

		return ao;
	}

#ifdef MODEL_COOK_TORRANCE
	float get_metallic()
	{
		#ifdef USE_VERTEX_COORD
			return read_texture(metallic_roughness_texture, v_coord).b;
		#endif
		return 0.0f;
	}

	float get_roughness()
	{
		#ifdef USE_VERTEX_COORD
			return read_texture(metallic_roughness_texture, v_coord).g;
		#endif
		return 0.0f;
	}

	vec3 fresnel_schlick(float cos_theta, vec3 f0)
	{
		return f0 + (1.0 - f0) * pow(1.0 - cos_theta, 5.0);
	}  

	float distribution_ggx(vec3 N, vec3 H, float roughness)
	{
		float a      = roughness*roughness;
		float a2     = a*a;
		float NdotH  = max(dot(N, H), 0.0);
		float NdotH2 = NdotH*NdotH;
	
		float num   = a2;
		float denom = (NdotH2 * (a2 - 1.0) + 1.0);
		denom = PI * denom * denom;
	
		return num / denom;
	}

	float geometry_schlick_ggx(float NdotV, float roughness)
	{
		float r = (roughness + 1.0);
		float k = (r*r) / 8.0;

		float num   = NdotV;
		float denom = NdotV * (1.0 - k) + k;
	
		return num / denom;
	}

	float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness)
	{
		float NdotV = max(dot(N, V), 0.0);
		float NdotL = max(dot(N, L), 0.0);
		float ggx2  = geometry_schlick_ggx(NdotV, roughness);
		float ggx1  = geometry_schlick_ggx(NdotL, roughness);
	
		return ggx1 * ggx2;
	}

	vec4 calculate_direct_light()
	{	
		vec3 light_pos = vec3(0, 0.7, 2.6);
		vec3 light_dir = normalize(light_pos - v_position); //TODO
		vec3 camera_position = u_camera_position;

		vec4 f_albedo = get_diffuse(); //TODO?
		float f_metallic = get_metallic();
		float f_roughness = get_roughness();
		vec3 f_normal = get_normal();

		vec3 f_v = normalize(v_position - camera_position);
		vec3 f_f0 = vec3(0.04f);
		f_f0 = mix(f_f0, f_albedo.rgb, f_metallic);

		vec3 f_l = light_dir;
		vec3 f_h = normalize(f_v + f_l);

		float distance    = length(light_pos - v_position);
        float attenuation = 1.0 / max(distance * distance, 0.001);
        vec3 radiance     = vec3(1.0, 0.839, 0.66) * vec3(10.5) * vec3(attenuation);        

		float f_ndf = distribution_ggx(f_normal, f_h, f_roughness);  
		float f_g   = geometry_smith(f_normal, f_v, f_l, f_roughness);  
		vec3 f_f = fresnel_schlick(max(dot(f_h, f_v), 0.0), f_f0);

		vec3 f_s = f_f;
        vec3 f_d = vec3(1.0) - f_s;
        f_d *= 1.0 - f_metallic;	  
        
        vec3 numerator    = f_ndf * f_g * f_f;
        float denominator = 4.0 * max(dot(f_normal, f_v), 0.0) * max(dot(f_normal, f_l), 0.0);
        vec3 specular     = numerator / max(denominator, 0.001);  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(f_normal, f_l), 0.0);   

		vec4 result = f_albedo;
        result.rgb = (f_d * f_albedo.rgb / PI + specular) * radiance * NdotL; 

		//vec3 ambient = vec3(0.03) * f_albedo.rgb;
		//result.rgb += ambient;

		return vec4(result);
	}
#endif

	void main()
	{
		vec4 result = calculate_direct_light() * vec4(get_ao(), 1.0);//vec4(f_normal*0.5 + 0.5, 1.0);

		//result.rgb = result.rgb / (result.rgb + vec3(1.0));
		result.rgb = vec3(1.0) - exp(-result.rgb);
		result.rgb = pow(result.rgb, vec3(1.0/gamma));

		output_color = result;
	} 
)";