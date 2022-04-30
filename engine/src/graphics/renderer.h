#pragma once

namespace Diamond {
	class Renderpass;

	class Renderer {
	public:
		void initialize();
		void cleanup();

		void beginFrame();
		void endFrame();
		void beginRenderpass(Renderpass* renderpass);
		void endRenderpass();
		void drawQuad();
	};
}