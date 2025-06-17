#pragma once

#include "Azer/Input.h"

namespace Azer {
	class WindowsInput : public Input
	{

	public:
		WindowsInput() = default;
	public:
		// Í¨¹ý Input ¼Ì³Ð
		bool IsKeyPressedImpl(int keycode) override;

		bool IsMouseButtonPressedImpl(int button) override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;

		std::pair<float, float> GetMousePosImpl() override;
	};
}