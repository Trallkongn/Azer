#type vertex

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Pos;

void main()
{
	gl_Position =  u_ViewProjection * u_Transform * vec4(a_Position,1.0);
}

#type fragment

uniform sampler2D equirectangularMap;

in vec3 v_Pos;

out vec4 FragColor;

void main()
{
	vec2 sampleUV = vec2(atan(v_Pos.z, v_Pos.x)/(2*PI)+0.5,asin(v_Pos.y)/PI + 0.5);
	vec3 color = texture(equirectangularMap, sampleUV).rgb;
	FragColor = vec4(color, 1.0);
}


