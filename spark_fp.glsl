#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in vec2 tex_coord;

uniform sampler2D texture_map;

void main (void)
{
    vec2 uv_use = 2*tex_coord;
    vec4 pixel = texture(texture_map, uv_use);

    // Use texture in determining fragment colour

    //currently using the color value, not the texture
    gl_FragColor = frag_color;
}
