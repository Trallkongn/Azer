#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    WorldPos = aPos;
    gl_Position = u_Projection * u_View * vec4(WorldPos, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap; // 之前生成的 cubemap

const float PI = 3.14159265359;

void main()
{
    vec3 N = normalize(WorldPos);

    vec3 irradiance = vec3(0.0);

    // 半球采样参数
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up         = cross(N, right);

    float sampleDelta = 0.025;  
    float nrSamples = 0.0;  

    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {
            // 球面到笛卡尔坐标
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    FragColor = vec4(irradiance, 1.0);
}
