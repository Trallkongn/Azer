#type vertex
#version 330 core 
 
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord; // 新增UV坐标 
 
uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;
 
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord; // 传递给片段着色器 
 
void main()
{
    gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
    FragPos = vec3(u_Transform * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_Transform))) * aNormal;
    TexCoord = aTexCoord; // 传递UV 
}

#type fragment
#version 330 core
 
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
 
uniform sampler2D mainTexture;
uniform sampler2D specularRamp;
uniform vec3 lightPos;
uniform vec3 viewPos;
 
out vec4 FragColor;
 
void main()
{
    // 1. 基础纹理 
    vec3 albedo = texture(mainTexture, TexCoord).rgb;
    
    // 2. 卡通漫反射
    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - FragPos);
    float diffuse = max(dot(N, L), 0.0);
    
    // 使用渐变贴图控制色阶
    vec3 diffuseColor = texture(specularRamp, vec2(diffuse, 0.5)).rgb;
    
    // 3. 卡通高光
    vec3 V = normalize(viewPos - FragPos);
    vec3 R = reflect(-L, N);
    float specular = pow(max(dot(V, R), 0.0), 32.0);
    specular = step(0.2, specular); // 二元化高光 
    
    // 4. 边缘光检测 
    float rim = 1.0 - max(dot(N, V), 0.0);
    rim = smoothstep(0.5, 0.8, rim); // 边缘光强度控制 
    
    // 合成 
    vec3 result = albedo * (diffuseColor + specular) + rim * vec3(0.8);
    FragColor = vec4(result, 1.0);
}