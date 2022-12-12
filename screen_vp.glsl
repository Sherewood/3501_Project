#version 400

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;
in float phase;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform float timer;

// Attributes forwarded to the geometry shader
out vec3 vertex_color;
out float timestep;
out vec2 uv_interp;



void main()
{
  
     vec4 position = world_mat * vec4(vertex, 1.0);
    vec4 norm = normal_mat * vec4(normal, 0.0);

}
