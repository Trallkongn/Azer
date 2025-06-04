#pragma once

#include "Azer/Layer.h"
#include "Azer/Events/Event.h"
#include "Azer/Events/MouseEvent.h"
#include "Azer/Events/KeyEvent.h"
#include "Azer/Events/ApplicationEvent.h"

namespace Azer {
	
	class AZER_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();

		~ImGuiLayer();


		void OnUpdate() override;


		void OnEvent(Event& event) override;


		void OnAttach() override;


		void OnDetach() override;
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
		bool OnKeyTypedEvent(Event& e);
	private:
		float m_Time = 0.0f;
	};
}
