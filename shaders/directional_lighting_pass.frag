#version 330

in vec2 v_Position;
in vec3 v_LightDir;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;
uniform vec3 u_LightColor;

out vec4 out_Color;

void main()
{
	vec3 diffuseColor = texture(u_DiffuseTexture, v_Position).rgb;
	vec3 normal = texture(u_NormalTexture, v_Position).xyz;
	vec3 position = texture(u_PositionTexture, v_Position).xyz;

	out_Color = vec4(diffuseColor * max(0, dot(normal, v_LightDir)) * u_LightColor, 1);
}
