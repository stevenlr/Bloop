#version 330

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TextureCoords;
in vec3 v_Tangent;

in vec3 v_LightDir;

out vec4 out_Color;

void main()
{
	const vec3 diffuseColor = vec3(0.95, 0.9, 0.7) * 0.8;
	const vec3 ambientColor = vec3(0.05, 0.05, 0.08);
	const vec3 specularColor = vec3(1, 1, 1);

	float diffuseFactor = max(0, dot(-v_LightDir, v_Normal));
	float specularFactor = pow(max(0, dot(reflect(-v_LightDir, v_Normal), normalize(v_Position))), 10);

	out_Color = vec4(ambientColor + diffuseColor * diffuseFactor + specularFactor * specularColor, 1);
}