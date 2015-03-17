#version 330

in vec3 v_Color;

out vec4 out_Color;

uniform float u_f;

void main()
{
	out_Color = vec4(v_Color, 1);
}