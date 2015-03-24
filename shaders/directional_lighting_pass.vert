#version 330

in vec2 in_Position;

out vec2 v_Position;
out vec3 v_LightDir;

uniform mat4 u_ViewMatrix;
uniform vec3 u_LightDir;

void main()
{
	gl_Position = vec4(in_Position * 2 - 1, 0, 1);
	v_Position = in_Position;
	v_LightDir = normalize((u_ViewMatrix * vec4(-u_LightDir, 0)).xyz);
}