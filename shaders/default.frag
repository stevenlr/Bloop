#version 330

out vec4 out_Color;

uniform float u_f;

void main()
{
	out_Color = vec4(u_f, 1, 1, 1);
}