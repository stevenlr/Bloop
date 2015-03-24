#version 330

in vec2 in_Position;

out vec2 v_Position;

void main()
{
	gl_Position = vec4(in_Position * 2 - 1, 0, 1);
	v_Position = in_Position;
}