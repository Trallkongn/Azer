#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Tex;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_Tex;

void main()
{
	v_Tex = a_Tex;
	gl_Position =  u_ViewProjection * u_Transform * vec4(a_Position,1.0);
}

#type fragment
#version 330 core
			
out vec4 FragColor;

in vec2 v_Tex;

uniform sampler2D u_TexSampler;

void main()
{
	FragColor = texture(u_TexSampler, v_Tex);
}