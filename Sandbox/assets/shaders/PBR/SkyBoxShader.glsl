#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;

out vec3 TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    TexCoords = a_Position;
    vec4 pos = u_Projection * u_View * vec4(a_Position, 1.0);
    gl_Position = pos.xyww; // w 分量重复一次，保证天空盒在最远处
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform bool hdr = true;                  // 是否使用HDR
uniform float exposure = 1.0;            // 曝光值，用于HDR色调映射

// 简单的 Reinhard 色调映射
vec3 reinhardToneMapping(vec3 color)
{
    float gamma = 2.2;
    vec3 mapped = color / (color + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / gamma));
    return mapped;
}

// 带曝光的 Reinhard 色调映射
vec3 reinhardWithExposure(vec3 color, float exposure)
{
    float gamma = 2.2;
    vec3 mapped = vec3(1.0) - exp(-color * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));
    return mapped;
}

void main()
{
    // 从立方体贴图采样颜色
    vec3 color = texture(skybox, TexCoords).rgb;
    
    // 如果是HDR天空盒，应用色调映射
    if (hdr)
    {
        color = reinhardWithExposure(color, exposure);
    }
    else
    {
        // LDR天空盒直接使用颜色
        float gamma = 2.2;
        color = pow(color, vec3(1.0 / gamma));
    }
    
    FragColor = vec4(color, 1.0);
}

