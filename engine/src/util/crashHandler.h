#pragma once

namespace Diamond {
	class CrashHandler {
	private:
		static std::string getTimeAndDateAsString();
		static void writeFullDump(EXCEPTION_POINTERS* exceptionPtrs, const std::string& name);
		static LONG WINAPI exceptionFilter(struct _EXCEPTION_POINTERS* exceptionInfo);

	public:
		static void initialize();
		static std::string getStackTrace();
	};
}