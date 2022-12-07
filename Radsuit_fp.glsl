#version 130

// Passed from the vertex shader
in vec2 uv0;

// Passed from outside
uniform float timer;
uniform sampler2D texture_map;

void main() 
{
vec4 pixel;
vec2 pos = uv0;
pixel = texture(texture_map,uv0);
	// ensures the screen effect reamins the same after a certain time 
	float s=min(4,timer);
	float cx= .6; //placement on the x axis. No need for any changes 
	float cy=3-.70*(s); //placement on the y axis, moves down slowly until the time maxes out
	float distance=pow((uv0.x-(cx/2)),2); //distance x equation changed a bit to better fit the game 
	float distancey=pow((uv0.y-(cy/1.5)),2); //distance y quation changed a bit so it fits the game 
	if (uv0.x > 3*distance && uv0.y > 1.5*distancey) //if conditional, all that fits in here becomes the offset effect 
	{
    // wavering
		
		vec2 pos = uv0;
	
		float offset  = 0.05*(sin(timer*.76+15*pos.y));
		pos.x = pos.x + offset;

		pixel = texture(texture_map, pos) ;
    }
	//color addition, moves downward mimicking putting on a suit 
	else if (uv0.y >=cy-2)
	{
	// if the coordinates are in the interior, colors the suit yellow 
		if (uv0.x <=.85 && uv0.x>=.08 && uv0.y<=.95 && uv0.y >=cy-.5)
		{
			pixel = vec4(1,.8,0,1);
		}
		// else, paints it white 
		else
		{
		pixel = vec4(1,1,1,0);
		}
	}
	

    gl_FragColor = pixel;
	
}
