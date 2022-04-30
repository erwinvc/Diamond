#include "stdafx.h"
#include "application.h"
#include "graphics/window.h"
#include "math/random.h"
#include "assets/loaders/modelLoader.h"
#include "src/GLError.h"
#include "src/assets/model.h"
#include "src/assets/mesh.h"
#include "src/graphics/buffers/indexBuffer.h"
#include "graphics/shader.h"
#include "graphics/framebuffer.h"
#include "ref.h"
#include "graphics/textureParameters.h"

namespace Diamond {
	void Application::initialize() {
		glfwSetErrorCallback(glErrorCallback);

		Random::initialize();
		Logger::Initialize("Diamond");

		if (!glfwInit()) {
			LOG_ERROR("[GLFW] GLFW failed to initialize");
			return;
		}

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		//glfwWindowHint(GLFW_SAMPLES, 8);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);

#ifdef D_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		m_mainWindow = new Window("Diamond", 1920, 1080);
		m_mainWindow->makeContextCurrent();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG_ERROR("[GLAD] failed to initialize");
			return;
		}

#if 0
		GL(glEnable(GL_DEBUG_OUTPUT));
		GL(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
		GL(glDebugMessageCallback(debugCallback, nullptr));
#endif
		m_mainWindow->getCallbacks().addOnResizeCallback(this, &Application::onResize);
		m_mainWindow->clearColor(Color(0.5f, 0.7f, 1.0f, 1.0f));
		m_mainWindow->setVSync(true);

		LOG("[~cGPU~x] %-26s %s", "GPU~1", glGetString(GL_RENDERER));
		LOG("[~cGPU~x] %-26s %s", "OpenGL version~1", glGetString(GL_VERSION));

		std::vector<StandardVertex> vertices;
		vertices.push_back(StandardVertex(glm::vec3(-1.f, -1.f, 0.0f), glm::vec3(0), glm::vec2(0)));
		vertices.push_back(StandardVertex(glm::vec3(-1.f, 1.f, 0.0f), glm::vec3(0), glm::vec2(0)));
		vertices.push_back(StandardVertex(glm::vec3(1.f, 1.f, 0.0f), glm::vec3(0), glm::vec2(0)));
		vertices.push_back(StandardVertex(glm::vec3(1.f, 1.f, 0.0f), glm::vec3(0), glm::vec2(0)));
		vertices.push_back(StandardVertex(glm::vec3(1.f, -1.f, 0.0f), glm::vec3(0), glm::vec2(0)));
		vertices.push_back(StandardVertex(glm::vec3(-1.f, -1.f, 0.0f), glm::vec3(0), glm::vec2(0)));

		std::vector<uint32_t> indices;
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(4);
		indices.push_back(5);

		Mesh mesh = Mesh("Test", vertices.data(), (uint32_t)vertices.size(), sizeof(StandardVertex), indices.data(), (uint32_t)indices.size());

		AssetRef<FrameBuffer> fbo = GetFrameBufferManager()->Create("test", ONEEIGHTH, Color::red());
		fbo->AddBuffer("Depth", TextureParameters(TextureFormat::DEPTH32F, NEAREST, CLAMP_TO_EDGE), FBOAttachment::DEPTH);
		auto m_colorTexture = fbo->AddBuffer("Color", TextureParameters(TextureFormat::RGBA, LINEAR, CLAMP_TO_EDGE));

		m_mainWindow->show();
		{

			//ModelLoader loader = ModelLoader("Model", "res/LTEO.obj", false);
			//loader.load();

			//Model* model = getAssetManager()->getAsset<Model>("Model");
			while (!m_mainWindow->shouldClose()) {
				m_mainWindow->clear();
				fbo->Bind();
				fbo->Clear();
				Shader shader("test", "res/shader/test", false, false);
				shader.bind();
				mesh.bind();
				GL(glDrawElements(GL_TRIANGLES, mesh.getIBO()->getCount(), GL_UNSIGNED_INT, NULL));
				fbo->BlitColorOnly(nullptr);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				GL(glViewport(0, 0, getWidth(), getHeight()));

				m_mainWindow->swapBuffers();
				m_mainWindow->pollEvents();
			}
		}
		delete m_mainWindow;
		glfwTerminate();
	}

	void Application::onResize(uint32_t with, uint32_t height) {
		glViewport(0, 0, with, height);
	}
}