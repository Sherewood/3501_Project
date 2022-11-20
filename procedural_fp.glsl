#version 400

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_pos;
in vec2 texCoord;

// Uniform (global) buffer
uniform sampler2D texture_map;
uniform float timer; 

float cols=10;
float rows=8;

void main() 
{


    vec2 uv_=uv_interp;

	uv_.x *= cols; // change number of columns
	uv_.y *= rows; // change number of rows
    vec3 blackOrWhite = vec3(mod(floor(uv_.x) + floor(uv_.y),2.0));
    uv_ =fract(uv_);
    vec3 color;
    
    // first texture
    vec3 color1 = vec3(0,0.9*(cos(timer)),0.1*(cos(timer)));
    // second texture
    vec3 color2 = vec3(1.0,.6,0.3);
       	// background color
    color = color1 * blackOrWhite + color2 * (1.0 - blackOrWhite);
    
	// Output to screen
	gl_FragColor = vec4(color,1.0);
    



}
