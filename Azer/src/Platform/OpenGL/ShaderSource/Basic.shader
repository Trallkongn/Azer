#shader vertex
#version 450 core

layout(location = 0) in vec3 position;

void main()
{
	gl_Position = position;
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.2,0.3,0.3,1.0);
}