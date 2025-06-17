#pragma once

#include "Azer/Input.h"

namespace Azer {
	class WindowsInput : public Input
	{

	public:
		WindowsInput() = default;
	public:
		// ͨ�� Input �̳�
		bool IsKeyPressedImpl(int keycode) override;

		bool IsMouseButtonPressedImpl(int button) override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;

		std::pair<float, float> GetMousePosImpl() override;
	};
}