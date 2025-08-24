#type vertex
#version 330 core 
 
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
 
out vec3 Normal;
out vec3 FragPos;
 
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}

#type fragment
#version 330 core
 
in vec3 Normal;
in vec3 FragPos;
 
out vec4 FragColor;
 
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
 
// 三渲二的色阶数量 
#define COLOR_BANDS 3
 
void main()
{
    // 环境光
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    
    // 三渲二效果 - 离散化漫反射
    float toonDiffuse = floor(diff * COLOR_BANDS) / (COLOR_BANDS - 0.5);
    vec3 diffuse = toonDiffuse * lightColor;
    
    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    // 三渲二效果 - 离散化高光
    float toonSpecular = (spec > 0.4) ? 1.0 : 0.0;
    vec3 specular = toonSpecular * lightColor;
    
    // 边缘光/轮廓线效果 
    float edge = dot(norm, viewDir);
    if (edge < 0.2) {
        edge = 0.0;
    } else {
        edge = 1.0;
    }
    
    // 最终颜色
    vec3 result = (ambient + diffuse + specular) * objectColor * edge;
    FragColor = vec4(result, 1.0);
}