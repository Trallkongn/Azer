#type vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

void main()
{
    FragPos = vec3(u_Transform * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_Transform))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
}


#type fragment
#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 camPos;

// 点光源
struct Light {
    vec3 Position;
    vec3 Color;
};
uniform Light lights[4];
uniform int lightCount;

// 材质
uniform sampler2D albedoMap;
uniform sampler2D metallicRoughnessMap; // G=roughness, B=metallic
uniform sampler2D aoMap;
// 补充的环境光照相关纹理
uniform samplerCube irradianceMap;    // 环境光照贴图（立方体贴图）
uniform samplerCube prefilterMap;     // 预过滤环境贴图（立方体贴图）
uniform sampler2D brdfLUT;            // BRDF查找表

const float PI = 3.14159265359;
// IBL相关参数
const float MAX_REFLECTION_LOD = 4.0;

// Fresnel
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// 用于IBL的Fresnel计算，带粗糙度参数
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

// GGX
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N,H),0.0);
    float NdotH2 = NdotH*NdotH;

    float num = a2;
    float denom = (NdotH2*(a2-1.0)+1.0);
    denom = PI*denom*denom;
    return num/denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness+1.0;
    float k = (r*r)/8.0;
    return NdotV/(NdotV*(1.0-k)+k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N,V),0.0);
    float NdotL = max(dot(N,L),0.0);
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - FragPos);
    vec3 R = reflect(-V, N);

    vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    vec4 mrSample = texture(metallicRoughnessMap, TexCoords);
    float metallic = mrSample.b;
    float roughness = mrSample.g;
    float ao = texture(aoMap, TexCoords).r;

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i=0;i<lightCount;i++)
    {
        vec3 L = normalize(lights[i].Position - FragPos);
        vec3 H = normalize(V+L);
        float distance = length(lights[i].Position - FragPos);
        float attenuation = 1.0 / (distance*distance);
        vec3 radiance = lights[i].Color * attenuation;

        float NDF = DistributionGGX(N,H,roughness);
        float G   = GeometrySmith(N,V,L,roughness);
        vec3 F    = fresnelSchlick(max(dot(H,V),0.0), F0);

        vec3 numerator = NDF*G*F;
        float denominator = 4.0*max(dot(N,V),0.0)*max(dot(N,L),0.0)+0.001;
        vec3 specular = numerator/denominator;

        vec3 kS = F;
        vec3 kD = (vec3(1.0)-kS)*(1.0-metallic);

        float NdotL = max(dot(N,L),0.0);
        Lo += (kD*albedo/PI + specular) * radiance * NdotL;
    }
    // IBL部分计算
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;
    
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;
    color = pow(color, vec3(1.0/2.2));
    FragColor = vec4(color,1.0);
}
