#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	gl_Position =  u_ViewProjection * u_Transform * vec4(a_Position,1.0);
}

#type fragment
#version 330 core
			
out vec4 FragColor;

uniform vec4 u_Color;

struct Material {
    vec3 albedo;
    vec3 F0;
    float metallic;
    float roughness;
    float ao;

    sampler2D textures[4]; // Normal, Diffuse, Specular, Roughness
};

uniform Material material;

void main()
{
    vec3 albedoColor = material.albedo;
    if (useDiffuseMap)
        albedoColor *= texture(material.textures[1], TexCoords).rgb;
	FragColor = u_Color;
}