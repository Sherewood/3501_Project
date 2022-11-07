#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_pos;
in vec3 vertex_;
// Uniform (global) buffer
uniform sampler2D texture_map;


float cols=10;
float rows=8;
void main() 
{
    // Retrieve texture value
    vec4 pixel = texture(texture_map, uv_interp);

//	pixel = vec4(0.3,0.4,0.6,1.0);
	//pixel = color_interp;
	vec4 lightcol = vec4(1,1,1,1);
	vec3 v = vertex_;
	vec3 vv = normalize(v - position_interp);
	vec3 l = vec3(-1,22,30);
	vec3 lv = normalize(l - position_interp);
	vec3 h = normalize((vv+lv)/2);
	vec3 n = normalize(normal_interp);
	vec3 r= (-lv)+2*(dot(lv,n))*n;//reflected vector, made with the normalized light vector)
	float diffuse = max(dot(n, lv), 0.0);
	float spec = max(0,dot(vv,r));

	spec = pow(spec,2);
	vec4 ambcol = vec4(1,0,0,1);
	//spec = 0;
	//diffuse = 0;
	float amb = 0.6;

	//pixel = vec4(0.4,0.8,0.9,1.0);
	    vec2 uv_=uv_interp;

	uv_.x *= cols; // change number of columns
	uv_.y *= rows; // change number of rows
    vec3 blackOrWhite = vec3(mod(floor(uv_.x) + floor(uv_.y),2.0));
    uv_ =fract(uv_);
    vec3 color;
    
    // first texture
    vec3 color1 = vec3(0,0.9,0.1);
    // second texture
    vec3 color2 = vec3(1.0,.6,0.3);
       	// background color
    color = color1 * blackOrWhite + color2 * (1.0 - blackOrWhite);
    
   
    gl_FragColor = (lightcol*pixel*diffuse +
	lightcol*vec4(1,1,1,1)*spec +
	lightcol*pixel*amb)*vec4(color,1.0);
		
   //gl_FragColor = vec4(1,position_interp.x,0,1);
}
