#pragma once

#include "azpch.h"

#include "Azer/Core/Core.h"
#include "Azer/Events/Event.h"

namespace Azer {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Azer Engine",
			uint32_t width = 1920,
			uint32_t height = 1080)
			: Title(title), Width(width), Height(height)
		{

		}
	};

	// Interface representing a desktop system based Window
	class AZER_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~ Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}