#pragma once

#include "Astro/Renderer/RendererAPI.h"

namespace Astro {

	class OpenGLRendererAPI : public RendererAPI {
		// Inherited via RendererAPI
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}