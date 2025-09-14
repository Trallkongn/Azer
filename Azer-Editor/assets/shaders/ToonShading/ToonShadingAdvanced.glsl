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
uniform vec3 viewPos;
uniform vec3 lightPos;           // 光源位置（不再使用，因为这是平行光源）
uniform vec3 sunDirection;       // 太阳光方向（平行光）
uniform vec3 ambientLight; // 新增：环境光
uniform float ambientStrength; // 新增：环境光强度
 
out vec4 FragColor;
 
void main()
{
       // 1. 纹理采样
    vec3 albedo = texture(mainTexture, TexCoord).rgb;

    // 2. 平行光源漫反射计算
    vec3 N = normalize(Normal);                     // 法线
    vec3 L = normalize(sunDirection);               // 太阳光方向（平行光源）
    float diffuse = max(dot(N, L), 0.0);            // 漫反射分量
    
    // 3. 高光（镜面反射）
    vec3 V = normalize(-FragPos);                   // 视角方向
    vec3 R = reflect(-L, N);                        // 反射方向
    float specular = pow(max(dot(V, R), 0.0), 32.0); // 高光计算

    // 4. 环境光
    vec3 ambient = ambientStrength * ambientLight;

    // 5. 合成最终颜色（环境光 + 漫反射 + 镜面反射）
    vec3 result = ambient + (albedo * diffuse) + specular * vec3(1.0, 1.0, 1.0);

    FragColor = vec4(result, 1.0);
}