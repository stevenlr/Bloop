#version 330

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TextureCoords;
in vec3 v_Tangent;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_NormalTexture;

out vec3 out_Color;
out vec3 out_Normal;
out vec3 out_Position;

vec3 computeNormal()
{
	vec3 normal = texture(u_NormalTexture, v_TextureCoords).rgb * 2 - 1;
	vec3 bitangent = cross(v_Tangent, v_Normal);

	return mat3(v_Tangent.x, v_Tangent.y, v_Tangent.z,
				bitangent.x, bitangent.y, bitangent.z,
				v_Normal.x, v_Normal.y, v_Normal.z) * normal;
}

void main()
{
	out_Color = texture(u_DiffuseTexture, v_TextureCoords).rgb;
	out_Normal = computeNormal();
	out_Position = v_Position;
}
