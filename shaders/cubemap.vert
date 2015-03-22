#version 330

in vec3 in_Position;

out vec3 v_Position;

uniform mat4 u_PvmMatrix;

void main()
{
	gl_Position = (u_PvmMatrix * vec4(in_Position * 1, 0)).xyww;
	v_Position = in_Position;
}
