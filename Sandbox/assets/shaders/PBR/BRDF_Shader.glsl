#type vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
}


#type fragment
#version 330 core

out vec2 FragColor;
in vec2 TexCoords;

const float PI = 3.14159265359;

// GGX / Schlick-Fresnel 的积分
vec2 IntegrateBRDF(float NdotV, float roughness)
{
    const int SAMPLE_COUNT = 1024;
    float A = 0.0;
    float B = 0.0;

    vec3 V = vec3(sqrt(1.0 - NdotV*NdotV), 0.0, NdotV);

    for(int i = 0; i < SAMPLE_COUNT; ++i)
    {
        // 使用 Hammersley / Importance Sampling
        vec2 Xi = vec2(float(i)/float(SAMPLE_COUNT), fract(sin(float(i)*12.9898)*43758.5453));
        float phi = 2.0 * PI * Xi.x;
        float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (roughness*roughness - 1.0) * Xi.y));
        float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

        vec3 H = vec3(cos(phi)*sinTheta, sin(phi)*sinTheta, cosTheta);
        vec3 L = normalize(2.0 * dot(V,H) * H - V);

        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V,H), 0.0);

        if(NdotL > 0.0)
        {
            float G = 2.0 * NdotH * NdotV / VdotH;
            float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G;
            B += Fc * G;
        }
    }
    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);
    return vec2(A, B);
}

void main()
{
    vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y);
    FragColor = integratedBRDF;
}
