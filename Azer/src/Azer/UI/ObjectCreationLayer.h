#pragma once

#include <Azer/Core/Core.h>
#include <Azer/Core/Layer.h>
#include <glm/glm.hpp>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer3D/PerspectiveGraphicCameraController.h>

namespace Azer {

	class VertexArray;

	struct GameObject
	{
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
		glm::vec3 color = { 1.0f, 1.0f, 1.0f };
		std::string name = "New Object";
	};

	class ObjectCreationLayer : public Layer
	{
	public:
		ObjectCreationLayer();
		~ObjectCreationLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep delta) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		void ShowObjectCreationWindow();
		void ShowObjectEditor();
		void RenderObjects();

		std::vector<GameObject> m_GameObjects;
		bool m_ShowCreationWindow = false;

	private:

		Ref<VertexArray> VAO;

		PerspectiveGraphicCameraController m_CameraController;
	};
}