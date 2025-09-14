#include "azpch.h"
#include "Cursor.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Azer/Renderer/Buffer.h>
#include <Azer/Renderer/RenderCommand.h>

#include <Azer/Mesh/VAOLibrary.h>
#include <Azer/Core/Application.h>

namespace Azer {

	Cursor::Cursor()
		: m_CamController(1.6/0.9,false)
	{
		m_VertexArray = VertexArray::Create();

		std::string vs = R"(
#version 330 core
layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_ClipPos;      // 只输出裁剪坐标即可

void main()
{
    vec4 pos    = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    gl_Position = pos;
    gl_PointSize = 32.0;   // 正方形点大小
    v_ClipPos   = pos;
}
		)";

		std::string fs = R"(
#version 330 core
layout(location = 0) out vec4 FragColor;

in vec4 v_ClipPos;               // 来自顶点着色器
uniform vec2 u_ViewportSize;     // 视口宽、高（像素）

void main()
{
    vec2 screenPos    = gl_FragCoord.xy;
    vec2 spriteCenter = (v_ClipPos.xy / v_ClipPos.w * 0.5 + 0.5) * u_ViewportSize;

    float radius = 16.0;                        // 圆半径（像素）
    vec2  offset = (screenPos - spriteCenter) / radius;

    if (dot(offset, offset) > 1.0)
        discard;

    // 用片段在圆内的归一化坐标做颜色，也可换成别的
    vec3 color = vec3(offset * 0.5 + 0.5, 0.5);
    FragColor  = vec4(color, 1.0);
}
		)";

		m_Shader = Shader::Create("pointShader", vs, fs);

		float vertices[3] = { 0.0f, 0.0f, 0.0f };
		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position"},
		};
		vbo->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vbo);
	}

	Cursor::~Cursor()
	{

	}

	void Cursor::Draw(PerspectiveGraphicCamera& camera, const glm::vec3& centerPos, const glm::vec2& viewportSize)
	{
		RenderCommand::CullFace(false);
		RenderCommand::DeepTest(false);

		m_Shader->Bind();
		m_Shader->SetMat4("u_ViewProjection",camera.GetViewProjectionMatrix());
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), centerPos);
		m_Shader->SetMat4("u_Transform", transform);
		m_Shader->SetFloat2("u_ViewportSize", viewportSize);

		m_VertexArray->Bind();
		RenderCommand::DrawPoints(m_VertexArray, 1);
		m_VertexArray->UnBind();

		RenderCommand::DeepTest(true);
	}
}

