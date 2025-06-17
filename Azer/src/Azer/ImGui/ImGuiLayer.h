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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}
