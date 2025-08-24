#type vertex
#version 330 core 
 
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord; // 新增UV坐标 
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
 
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord; // 传递给片段着色器 
 
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord; // 传递UV 
}

#type fragment
#version 330 core 
 
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord; // 接收UV 
 
uniform sampler2D mainTexture; // 主纹理
uniform sampler2D rampTexture; // 渐变贴图（可选）
uniform vec3 lightPos;
uniform vec3 viewPos;
 
out vec4 FragColor;
 
void main()
{
    // 基础光照计算
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    
    // 方法1：硬编码色阶
    float toon = floor(diff * 3.0) / 2.0; // 3阶色阶 
    
    // 方法2：使用渐变贴图（更灵活）
    // vec3 rampColor = texture(rampTexture, vec2(diff, 0.5)).rgb;
    
    // 最终颜色 
    vec3 baseColor = texture(mainTexture, TexCoord).rgb;
    FragColor = vec4(baseColor * toon, 1.0);
} 