#version 430

in vec2 UV;

out vec4 daColor;

uniform sampler2D myTextureSampler;

void main()
{
	vec3 Material_Clr = texture( myTextureSampler, UV).rgb;

	daColor = vec4(Material_Clr, 1.0);
}
