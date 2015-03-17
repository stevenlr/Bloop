#version 330

in vec3 in_Position;
in vec3 in_Color;

out vec3 v_Color;

uniform mat4 u_PvmMatrix;

void main()
{
	gl_Position = u_PvmMatrix * vec4(in_Position, 1);
	v_Color = in_Color;
}