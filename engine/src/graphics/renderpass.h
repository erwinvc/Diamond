#pragma once

#include "framebuffer.h"

namespace Diamond {
	class Renderpass {


	public:
		Renderpass(Framebuffer* _framebuffer, std::string& _name) : framebuffer(_framebuffer), name(_name) {}
private:
	Framebuffer* framebuffer;
	std::string name;
};
}

