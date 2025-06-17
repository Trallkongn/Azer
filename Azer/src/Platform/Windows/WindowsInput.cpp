#include "azpch.h"
#include "WindowsInput.h"

#include "GLFW/glfw3.h"
#include "Azer/Application.h"

Azer::Input* Azer::Input::s_Instance = new WindowsInput();

bool Azer::WindowsInput::IsKeyPressedImpl(int keycode)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Azer::WindowsInput::IsMouseButtonPressedImpl(int button)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

float Azer::WindowsInput::GetMouseXImpl()
{
	// c++ 17
	auto [x, y] = GetMousePos();
	return x;
}

float Azer::WindowsInput::GetMouseYImpl()
{
	auto [x, y] = GetMousePos();
	return y;
}

std::pair<float, float> Azer::WindowsInput::GetMousePosImpl()
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	return std::make_pair<float, float>((float)xPos, (float)yPos);
}
