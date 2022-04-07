#pragma once
#include <consoleapi2.h>
#include <mutex>
#include <queue>

namespace Diamond {
	#define LOG( fmt, ...)					Logger::Message((int)ConsoleColor::WHITE,				" [Info]",		fmt, ##__VA_ARGS__ )
	#define LOG_WARN( fmt, ...)				Logger::Message((int)ConsoleColor::RED,					" [Warn]",		fmt, ##__VA_ARGS__ )
	#define LOG_ERROR( fmt, ...)			{Logger::Message((int)ConsoleColor::RED,				" [Fail]",		fmt, ##__VA_ARGS__ ); Logger::ForceEmptyQueue(); __debugbreak();}

	class Thread;

	enum class ConsoleColor {
		BLACK = 0,
		DARKBLUE = FOREGROUND_BLUE,
		DARKGREEN = FOREGROUND_GREEN,
		DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
		DARKRED = FOREGROUND_RED,
		DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
		DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
		DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		GRAY = FOREGROUND_INTENSITY,
		BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
		MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		RESET = 16
	};

	/*Non-blocking logger with color formatting*/
	class Logger {
	public:
		struct QueuedMessage {
			int m_color;
			std::string m_message;
			std::string m_type;
			time_t m_time;
			QueuedMessage(int color, std::string message, std::string type, time_t time) : m_color(color), m_message(message), m_type(type), m_time(time) {}
			QueuedMessage() : m_color(0), m_message(""), m_type(""), m_time(0) {}
		};
		static const int MAXQUEUESIZE = 1000;
		static void Initialize(const char* title);
		static void Message(int color, const char* type, const char* fmt, ...);
		static void ForceEmptyQueue();
		static void Cleanup();

	private:
		static const char* s_logPath;

		static FILE* s_stream;
		static HANDLE s_outputHandle;
		static HANDLE s_inputHandle;
		static Thread* s_inputThread;
		static Thread* s_outputThread;
		static CONSOLE_SCREEN_BUFFER_INFO s_screenBuffer;
		static bool s_initialized;
		static bool m_shuttingDown;

		static void LogToFile(const char* buff);
		static void SetTextColor(const int color);
		static const char* GetTimeAsString(time_t& currentTime);

		//Queue
		static std::mutex m_mutex;
		static std::condition_variable m_conditionVariable;
		static std::queue<QueuedMessage> m_queue;
		static void HandleQueue();
		static void AddToQueue(int color, const std::string& message, const std::string& type, time_t time);
		static void ProcessMessage(QueuedMessage& message);

	};
}