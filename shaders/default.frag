#version 330

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TextureCoords;
in vec3 v_Tangent;
in vec3 v_LightDir;

uniform mat4 u_InverseViewMatrix;
uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_SpecularTexture;
uniform samplerCube u_Cubemap;

out vec4 out_Color;

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

	const vec3 specularColor = vec3(1, 1, 1);

	float ambientFactor = 0.5;
	float diffuseFactor = max(0, dot(-v_LightDir, normal)) * (1 - ambientFactor);
	float specularFactor = pow(max(0, dot(reflect(-v_LightDir, normal), normalize(v_Position))), 10) * specularIntensity;

	specularFactor = (specularFactor + 0.15) / 1.15;

	vec3 reflected = (u_InverseViewMatrix * vec4(reflect(normalize(v_Position), normal), 0)).xzy * vec3(1, -1, 1);
	vec3 cubemapReflectColor = textureLod(u_Cubemap, reflected, 4).rgb;
	vec3 cubemapAmbientColor = textureLod(u_Cubemap, (u_InverseViewMatrix * vec4(v_Normal, 0)).xzy * vec3(1, -1, 1), 8).rgb;

	float rimFactor = pow(1 - abs(dot(v_Normal, v_LightDir)), 1) * (dot(v_LightDir, vec3(0, 0, 1)) + 1) / 2;

	out_Color = vec4(diffuseColor * cubemapAmbientColor * (diffuseFactor + ambientFactor + rimFactor) + specularFactor * cubemapReflectColor, 1);
}
