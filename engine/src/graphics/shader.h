#pragma once
#include "util/path.h"
#include "util/color.h"
#include "graphics/shaderProgram.h"
#include "shaderUniformBuffer.h"

namespace Diamond {
	class Shader {
	private:
		ShaderProgram* m_shaderProgram;
		ShaderUniformBuffer m_uniformBuffer;

		bool m_hasGeometry;
		bool m_hasTessellation;
		std::string m_name;
		std::string m_shaderPath;

		uint32_t loadShader(const std::string& path, uint32_t type);
		bool addShaderToProgram(ShaderProgram* program, const std::string& shader, uint32_t shaderType);

		ShaderProgram* load();

		friend class ShaderManager;

	public:
		Shader(const std::string& name, const std::string& filePath, bool hasGeometry = false, bool hasTessellation = false);
		~Shader();

		//static uint32_t s_boundShader;

		inline const std::string& getName() { return m_name; }

		template<typename T>
		void Set(const std::string& location, const T* value, uint32_t count) {
			m_uniformBuffer.Set(location, value, count);
		}

		template<typename T>
		void Set(const std::string& location, const T& value) {
			m_uniformBuffer.Set(location, value);
		}

		template <>
		void Set<bool>(const std::string& location, const bool& value) {
			m_uniformBuffer.Set(location, (int)value);
		}

		void reload();
		void bind();
	};
}