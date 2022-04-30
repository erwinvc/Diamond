#pragma once
#include "stdincl.h"

namespace Diamond {
	class Framebuffer;

	class Renderpass {
	private:
		Framebuffer* framebuffer;
		std::string name;

	public:
		Renderpass(Framebuffer* _framebuffer, std::string& _name) : framebuffer(_framebuffer), name(_name) {}
	};
}

