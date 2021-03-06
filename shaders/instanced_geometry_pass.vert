#version 330

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TextureCoords;
in vec3 in_Tangent;
in vec3 in_InstancePosition;

uniform mat4 u_PvmMatrix;
uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ViewMatrix;
uniform mat3 u_NormalMatrix;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TextureCoords;
out vec3 v_Tangent;

void main()
{
	gl_Position = u_PvmMatrix * vec4(in_Position + in_InstancePosition, 1);

	v_Position = (u_ModelViewMatrix * vec4(in_Position + in_InstancePosition, 1)).xyz;
	v_Normal = normalize(u_NormalMatrix * in_Normal);
	v_TextureCoords = in_TextureCoords;
	v_Tangent = normalize((u_ModelViewMatrix * vec4(in_Tangent, 1)).xyz);
}
