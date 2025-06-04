#include "azpch.h"
#include "ImGuiLayer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Azer/Application.h"

#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Platform/OpenGL/imgui_impl_glfw.h"

#include "Azer/Core.h"

Azer::ImGuiLayer::ImGuiLayer()
	:Layer("ImGuiLayer")
{

}

Azer::ImGuiLayer::~ImGuiLayer()
{

}

void Azer::ImGuiLayer::OnUpdate()
{
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();
	io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

	float time = (float)glfwGetTime();
	io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
	m_Time = time;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Azer::ImGuiLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
}

void Azer::ImGuiLayer::OnAttach()
{

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	ImGui_ImplOpenGL3_Init("#version 410");

	// set imgui callback by imgui system
	GLFWwindow* m_Window = glfwGetCurrentContext();
	ImGui_ImplGlfw_InitForOpenGL(m_Window, 1);
}

void Azer::ImGuiLayer::OnDetach()
{
	//throw std::logic_error("The method or operation is not implemented.");
}

bool Azer::ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
	return false;
}

bool Azer::ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
	return false;
}

bool Azer::ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
{
	return false;
}

bool Azer::ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
{
	return false;
}

bool Azer::ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
{
	return false;
}

bool Azer::ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
{
	return false;
}

bool Azer::ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
{
	return false;
}

bool Azer::ImGuiLayer::OnKeyTypedEvent(Event& e)
{
	return false;
}
