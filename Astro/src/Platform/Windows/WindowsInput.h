#pragma once

#include "Astro/Input.h"

namespace Astro {

	class ASTRO_API WindowsInput: public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode);

		virtual bool IsMouseButtonPressedImpl(int button) override;

		virtual std::pair<float, float> GetMousePositionImpl() override;

		virtual float GetMouseXImpl() override;

		virtual float GetMouseYImpl() override;

	};

}