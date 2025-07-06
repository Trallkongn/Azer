#pragma once

#include "azpch.h"

#include "Core.h"
#include "Azer/Events/Event.h"
#include "Window.h"
#include "Azer/Events/ApplicationEvent.h"
#include "Azer/LayerStack.h"

#include "Azer/ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/OrthoGraphicCamera.h"

namespace Azer {

	class AZER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool onWindowClosed(WindowCloseEvent& e);	
		bool onKeyPressed(KeyPressedEvent& e);	
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer; // Azer's ImGui
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<Shader> m_Shader;

		OrthoGraphicCamera m_Camera;
		float m_Rotation = 0.0f;
		float x = 0.0f, y = 0.0f, z = 0.0f;
	private:
		static Application* s_Instance;

	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
