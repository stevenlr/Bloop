#version 330

in vec3 in_Position;

uniform mat4 u_PvmMatrix;
uniform vec4 u_Position;
uniform vec3 u_Scale;

void main()
{
	gl_Position = u_PvmMatrix * vec4(in_Position * u_Scale / 5 + u_Position.xyz, 1);
}