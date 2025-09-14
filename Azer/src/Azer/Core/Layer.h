#pragma once

#include "Azer/Core/Core.h"
#include "Azer/Events/Event.h"
#include "Azer/Core/TimeStep.h"

#include <string>

namespace Azer {

	class AZER_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep delta) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}


