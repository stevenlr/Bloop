#version 330

in vec2 v_Position;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;

out vec4 out_Color;

void main()
{
	vec3 diffuseColor = texture2D(u_DiffuseTexture, v_Position).rgb;
	vec3 normal = texture2D(u_NormalTexture, v_Position).xyz;
	vec3 position = texture2D(u_PositionTexture, v_Position).xyz;

	out_Color = vec4(texture2D(u_DiffuseTexture, v_Position).rgb * dot(normal, vec3(0, 1, 0)), 1);
}