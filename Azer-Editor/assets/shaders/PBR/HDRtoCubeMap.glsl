#type vertex
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 vDir; // 传给片段着色器的方向向量

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    vDir = aPos; // 顶点位置作为方向向量
    gl_Position = u_Projection * u_View * vec4(aPos, 1.0);
    gl_Position.z = gl_Position.w * 0.9999; // 深度值设为接近远平面 (1.0)
}

#type fragment
#version 330 core

out vec4 FragColor;

in vec3 vDir;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183); // 1/(2π), 1/π

vec2 sampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = sampleSphericalMap(normalize(vDir));
    vec3 color = texture(equirectangularMap, uv).rgb;
    
    FragColor = vec4(color, 1.0);
    //FragColor = vec4(1.0,0.0,0.0,1.0);
}


