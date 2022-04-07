#pragma once

namespace Diamond {
	template<typename T>
	class Singleton {
	protected:
		Singleton() = default;
		~Singleton() = default;

	public:
		static T* getInstance() {
			static T instance;
			return &instance;
		}

		Singleton(Singleton const&) = delete;
		Singleton& operator=(Singleton const&) = delete;
	};
}