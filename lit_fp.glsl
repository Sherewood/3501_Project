#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_dir;
in vec3 light_vertex;
// Uniform (global) buffer
uniform sampler2D texture_map;


void main() 
{
    // Retrieve texture value
    vec4 pixel = texture(texture_map, uv_interp);

	vec4 lightcol = vec4(1,1,1,1);
	vec3 v = light_vertex; //changed to take in a position in the world 
	vec3 vv = normalize(v - position_interp); //normalized vector 
	vec3 l = normalize(light_dir-position_interp); //light direction
	vec3 lv = normalize(l - position_interp);
	vec3 h = normalize((vv+lv)/2); 
	vec3 n = normalize(normal_interp);
	vec3 r= (-lv)+2*(dot(lv,n))*n; //reflected vector, made with the normalized light vector)
	float diffuse = max(dot(n, lv), 0.0);
	float spec = max(0,dot(vv,r));

	spec = pow(spec,2);
	vec4 ambcol = vec4(1,0,0,1);
	float amb = 0.6;

   
    gl_FragColor = lightcol*pixel*diffuse +
	lightcol*vec4(1,1,1,1)*spec +
	lightcol*pixel*amb;
}
