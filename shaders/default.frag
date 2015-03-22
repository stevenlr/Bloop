#version 330

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TextureCoords;
in vec3 v_Tangent;

in vec3 v_LightDir;

out vec4 out_Color;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_SpecularTexture;

vec3 computeNormal()
{
	vec3 normal = texture2D(u_NormalTexture, v_TextureCoords).rgb * 2 - 1;
	vec3 bitangent = cross(v_Tangent, v_Normal);

	return mat3(v_Tangent.x, v_Tangent.y, v_Tangent.z,
				bitangent.x, bitangent.y, bitangent.z,
				v_Normal.x, v_Normal.y, v_Normal.z) * normal;
}

void main()
{
	vec3 normal = computeNormal();

	vec3 diffuseColor = texture2D(u_DiffuseTexture, v_TextureCoords).rgb;
	float specularIntensity = texture2D(u_SpecularTexture, v_TextureCoords).r;

	const vec3 diffuseLightColor = vec3(0.95, 0.9, 0.7) * 0.8;
	const vec3 ambientLightColor = vec3(0.05, 0.05, 0.08);
	const vec3 specularColor = vec3(1, 1, 1);

	float diffuseFactor = max(0, dot(-v_LightDir, normal));
	float specularFactor = pow(max(0, dot(reflect(-v_LightDir, normal), normalize(v_Position))), 10) * specularIntensity;

	out_Color = vec4(diffuseColor * (ambientLightColor + diffuseLightColor * diffuseFactor) + specularFactor * specularColor, 1);
}