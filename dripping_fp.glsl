#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in vec2 uv_interp;
uniform sampler2D texture_map;

void main ()
{
    //float z= uv_coord[0];
    vec4 pixel= texture(texture_map,uv_interp);
    // Very simple fragment shader, but we can do anything we want here
    // We could apply a texture to the particle, illumination, etc.
    gl_FragColor = pixel;
}
//