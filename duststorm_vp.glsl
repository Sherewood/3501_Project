#version 400

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;
in float phase;
//in float cycle_length;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform float timer;

// Attributes forwarded to the geometry shader
out vec3 vertex_color;
out float timestep;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0);
uniform vec3 object_color = vec3(0.8, 0.8, 0.8);
float grav = 0.20; // Gravity
float speed = 6.00; // Controls the speed of the explosion
float upward = normalize(5); // additional y velocity for all particles
//float phase2=0;//timer - 7 * floor(timer / 7);
float cycle_length =10;
//prof notes: Make the movement more complex(write an equation for the curves), implenet phase in vertex processor,  

void main()
{
    //float circtime = timer - 6 * floor(timer / 6);
    float t = mod((timer+phase),cycle_length); // Our time parameter
    
    // Let's first work in model space (apply only world matrix)
    vec4 position = world_mat * vec4(vertex, 1.0);
    vec4 norm = normal_mat * vec4(normal, 0.0);
    // Move point along normal and down with t*t (acceleration under gravity)
    position.x += (norm.x)*t*speed+(2*cos(t)) - grav*speed*up_vec.x*t*t;
	
	// add "upward" to y speed to launch the particles vertically -- can easily omit
    position.y += (upward+norm.y)*t*speed+sin(t*2)- (grav*speed*up_vec.y*t*t);

    position.z += norm.z*t*speed - grav*speed*up_vec.z*t*t;
    
    // Now apply view transformation
    gl_Position = view_mat * position;
        
    // Define outputs
    // Define color of vertex
    vertex_color= vec3(.761, .698, .502);

    // Forward time step to geometry shader
    timestep = timer;

}
