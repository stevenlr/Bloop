#version 330

in vec3 v_Position;

out vec4 out_Color;

uniform samplerCube u_Cubemap;

void main()
{
	vec3 cubemapColor = texture(u_Cubemap, normalize(v_Position).xzy * vec3(1, -1, 1)).rgb;
	out_Color = vec4(cubemapColor, 1);
}
