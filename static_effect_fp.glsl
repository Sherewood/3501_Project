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
//generate a random number between 0 and 1 using the x and y values of the fragment
float randomNum = mod((pos.x*177223801 + 6319861 * pos.y)/48382 + timer*300.141289,1);

	//calculate an offset for the fragment with a sin function
	float offset  = 0.05*(sin(timer*0.0+8.0*pos.y));
	offset = offset*2 ;
	//add the offset to the x and y values
    pos.x = pos.x + offset;
	pos.y = pos.y + offset;
	
	pixel = texture(texture_map, pos) ;

	//create a greyscale colour for the fragment
	float g = (pixel.r+pixel.g+pixel.b)/3.0;

	//increase or decrease the brightness of the fragment based on the random number
	if(randomNum > 0.5){
		g += 0.3;
	}
	else {
		g -= 0.3;
	}
	//set the colour to the greyscale colour
	pixel = vec4(g,g,g,1);
	
    gl_FragColor = pixel;
}
