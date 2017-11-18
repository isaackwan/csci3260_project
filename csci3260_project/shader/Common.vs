#version 430

in layout(location = 0) vec3 vertexPosition_modelspace;
in layout(location = 1) vec2 vertexUV;
in layout(location = 2) vec3 normal;

uniform mat4 PM;
uniform mat4 MM;
uniform mat4 VM;

out vec3 normalWorld;
out vec2 UV;

void main()
{
    vec4 v = vec4(vertexPosition_modelspace, 1);
	gl_Position = PM * VM * MM * v;
	
	vec4 normal_temp = MM * vec4(normal, 0);
	normalWorld = normal_temp.xyz;

	UV = vertexUV;
}