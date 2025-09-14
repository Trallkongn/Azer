#include "azpch.h"
#include "WorldGrid.h"
#include <glm/glm.hpp>
#include <Azer/Renderer/RenderCommand.h>

namespace Azer {

	WorldGrid::WorldGrid(float halfExtent, float spacing)
	{
		m_VertexArray = VertexArray::Create();

        std::vector<glm::vec3> lines;
        // 平行于 x 的线
        for (float z = -halfExtent; z <= halfExtent + 1e-4f; z += spacing)
        {
            lines.emplace_back(-halfExtent, 0.0f, z);
            lines.emplace_back(halfExtent, 0.0f, z);
        }
        // 平行于 z 的线
        for (float x = -halfExtent; x <= halfExtent + 1e-4f; x += spacing)
        {
            lines.emplace_back(x, 0.0f, -halfExtent);
            lines.emplace_back(x, 0.0f, halfExtent);
        }
        m_LineCounts = lines.size();

        Ref<VertexBuffer> vbo = VertexBuffer::Create(lines.data(), lines.size() * sizeof(glm::vec3));
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position"}
        };
        vbo->SetLayout(layout);

        m_VertexArray->AddVertexBuffer(vbo);

        std::string vertexShader = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            uniform mat4 u_ViewProjection;
            void main()
            {
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
            )";

        std::string fragmentShader = R"(
            #version 330 core
            layout(location = 0) out vec4 FragColor;
            void main()
            {
                FragColor = vec4(0.4,0.4,0.4,1.0);
            }
            )";

        m_Shader = Shader::Create("GridShader", vertexShader, fragmentShader);
	}

	WorldGrid::~WorldGrid()
	{

	}

    void WorldGrid::Draw(PerspectiveGraphicCamera& camera)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        m_VertexArray->Bind();
        RenderCommand::DrawLine(m_VertexArray, m_LineCounts);
        m_VertexArray->UnBind();
    }
}