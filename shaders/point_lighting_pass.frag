#version 330

in vec2 v_Position;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;

uniform vec3 u_LightColor;
uniform vec4 u_LightPosition;
uniform vec3 u_LightIntensity;

out vec4 out_Color;

void main()
{
	vec3 diffuseColor = texture(u_DiffuseTexture, v_Position).rgb;
	vec3 normal = texture(u_NormalTexture, v_Position).xyz;
	vec3 position = texture(u_PositionTexture, v_Position).xyz;

	vec3 lightDir = u_LightPosition.xyz - position;

	float length = length(lightDir);
	lightDir = lightDir / length;

	float diffusefactor = max(0, dot(lightDir, normal)) / (1 + pow(length, 2));

	out_Color = vec4(u_LightColor * diffuseColor * diffusefactor, 1);
}
