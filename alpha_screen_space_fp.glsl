#version 130

// Passed from the vertex shader
in vec2 uv0;

// Passed from outside
uniform float timer;
uniform sampler2D texture_map;
uniform bool newEffect;

void main() 
{
vec4 pixel;
vec2 pos = uv0;
pixel = texture(texture_map,uv0);
vec4 sum = vec4(1.0); //set to 1 so it doesn't ruin math while the effect is off
	

	if (!newEffect){	
	//	if (uv0.x > 0.5 && uv0.y > 0.5)
		{
		// wavering
		vec2 pos = uv0;
	
		float offset  = 0.05*(sin(timer*0.0+8.0*pos.y));
		pos.x = pos.x + offset;

		pixel = texture(texture_map, pos) ;
		}
	
		if (uv0.x > 0.5 && uv0.y < 0.5)
		{
		// grey
		float g = (pixel.r+pixel.g+pixel.b)/3.0;
		pixel = vec4(g*.6,g,g*0.7,1);
		}
	
		if (uv0.x < 0.5 && uv0.y > 0.5)
		{
		// edges
		vec4 offx = texture(texture_map,uv0+vec2(0.005,0));
		vec4 offy = texture(texture_map,uv0+vec2(0,0.005));
		vec4 diff = abs(pixel-offx)+abs(pixel-offy);
		float edginess = diff.r+diff.g+diff.b;
		if (edginess > 1.8)
			pixel = vec4(0.8,0.9,1.0,1.0);
		else
			pixel = vec4(0,0,0,1);
		}
	
		if (uv0.x < 0.5 && uv0.y < 0.5)
		{
		// tv blockiness
		float spacing = 0.01;
		float thick = 0.005;
		if ((mod(uv0.y, spacing) < thick) || (mod(uv0.x,spacing) < thick))
			pixel = vec4(0,0,0,1);
		}
	}
	else{
		float limitedTimer = min(timer, 15.0); //caps out the ellipse closing in so that it doesn't cover all of the vision
		if (pow(uv0.x-0.5,2)/pow(1-limitedTimer/40,2) + pow(uv0.y-0.5,2)/pow(1-limitedTimer/30,2) > 0.5){ //formula for checking if outside ellipse centered at 0.5,0.5
			//this creates the weird pattern for the outside of the ellipse
			pixel = vec4(
			float(20+mod(timer*(uv0.x-0.5)/(uv0.y-0.5)*1000,20))/float(255),
			float(90+5*mod(timer*(uv0.x-0.5)*(uv0.y-0.5)*1000,20))/float(255),
			float(20+mod(timer/(uv0.x-0.5)*(uv0.y-0.5)*1000,20))/float(255),
			1.0);
		}
		else{ //if it's inside the ellipse
			sum = vec4(0.0); //reset to 0 since it needs to be actually used now
			//the amount to blur, i.e. how far off center to sample from 
			float blur = timer/150; 
			//apply blurring, using a 9-tap filter with predefined gaussian weights
			//i'm pretty sure these numbers are just random but they work pretty well
			sum += texture2D(texture_map, vec2(uv0.x - 4.0*blur, uv0.y - 4.0*blur)) * 0.0162162162;
			sum += texture2D(texture_map, vec2(uv0.x - 3.0*blur, uv0.y - 3.0*blur)) * 0.0540540541;
			sum += texture2D(texture_map, vec2(uv0.x - 2.0*blur, uv0.y - 2.0*blur)) * 0.1216216216;
			sum += texture2D(texture_map, vec2(uv0.x - 1.0*blur, uv0.y - 1.0*blur)) * 0.1945945946;
    
			sum += texture2D(texture_map, vec2(uv0.x, uv0.y)) * 0.2270270270;
    
			sum += texture2D(texture_map, vec2(uv0.x + 1.0*blur, uv0.y + 1.0*blur)) * 0.1945945946;
			sum += texture2D(texture_map, vec2(uv0.x + 2.0*blur, uv0.y + 2.0*blur)) * 0.1216216216;
			sum += texture2D(texture_map, vec2(uv0.x + 3.0*blur, uv0.y + 3.0*blur)) * 0.0540540541;
			sum += texture2D(texture_map, vec2(uv0.x + 4.0*blur, uv0.y + 4.0*blur)) * 0.0162162162;
		}
	}
    gl_FragColor = pixel*sum;
}
