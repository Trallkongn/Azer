#pragma once
#include "RenderCommand.h"
#include "GraphicCamera.h"
#include "Shader.h"

namespace Azer {

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(GraphicCamera& camera);
		static void EndScene();

		static void Submit(const Azer::Ref<VertexArray>& vertexArray, const Azer::Ref<Shader> shader, const glm::mat4& transform = glm::mat4(1.0f));

		static void Shutdown();

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}