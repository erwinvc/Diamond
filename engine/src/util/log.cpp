#include "stdafx.h"
#include "log.h"
#include "application.h"
#include "util/utils.h"
#include "util/thread/threadManager.h"

namespace Diamond {
	bool Logger::s_initialized = false;
	bool Logger::m_shuttingDown = false;
	const char* Logger::s_logPath = "log.log";


	HANDLE Logger::s_outputHandle = INVALID_HANDLE_VALUE;
	HANDLE Logger::s_inputHandle = INVALID_HANDLE_VALUE;
	Thread* Logger::s_inputThread;
	Thread* Logger::s_outputThread;
	CONSOLE_SCREEN_BUFFER_INFO Logger::s_screenBuffer;

	std::mutex Logger::m_mutex;
	std::condition_variable Logger::m_conditionVariable;
	std::queue<Logger::QueuedMessage> Logger::m_queue;

	void HandleInput() {
		static std::string input;
		std::getline(std::cin, input);
		LOG("~R%s", input.c_str());
		if (input.compare("exit") == 0 || input.compare("shutdown") == 0) {
			getApp()->cleanup();
		}
	}

	/*Retrieve QueuedMessages from the queue (async)*/
	void Logger::HandleQueue() {
		static QueuedMessage message;
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			while (m_queue.empty()) m_conditionVariable.wait(lock);
			if (m_queue.empty())return;
			message = std::move(m_queue.front());
			m_queue.pop();
		}
		ProcessMessage(message);
	}

	/*Initialize the non-blocking logger*/
	void Logger::Initialize(const char* title) {
		if (s_initialized) return;

		AllocConsole();
		SetConsoleTitleA(title);

		s_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		s_inputHandle = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleScreenBufferInfo(s_outputHandle, &s_screenBuffer);

		const int width = 120;
		const int height = 30;

		s_initialized = true;

		LOG("[~2Logging~x] console allocated");

		s_inputThread = GetThreadManager()->registerThread("Console input", HandleInput, true);
		s_outputThread = GetThreadManager()->registerThread("Console output", HandleQueue);

		std::ofstream ofs;
		ofs.open(s_logPath, std::ofstream::out | std::ofstream::trunc);
		ofs.close();
	}

	/*Set the text color of the next print to console*/
	void Logger::SetTextColor(const int color) {
		WORD attributes = s_screenBuffer.wAttributes & ~FOREGROUND_RED & ~FOREGROUND_GREEN & ~FOREGROUND_BLUE & ~FOREGROUND_INTENSITY;
		attributes |= color;
		SetConsoleTextAttribute(s_outputHandle, attributes);
	}

	/*Get the time as a printable string*/
	const char* Logger::GetTimeAsString(time_t& currentTime) {
		struct tm timeStruct;
		localtime_s(&timeStruct, &currentTime);

		static char buffer[48];
		sprintf_s(buffer, "[%02d:%02d:%02d]", timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec);
		return buffer;
	}

	/*Add QueuedMessage to the async queue*/
	void Logger::AddToQueue(int color, const std::string& message, const std::string& type, time_t time) {
		std::unique_lock<std::mutex> l(m_mutex);
		m_queue.emplace(std::move(QueuedMessage(color, message, type, time)));
		m_conditionVariable.notify_one();
	}

	/*Process a QueuedMessage (async)*/
	void Logger::ProcessMessage(QueuedMessage& message) {
		const char* formattedTime = GetTimeAsString(message.m_time);
		std::string outputString;
		std::string formattedMessage = StringUtils::replace(message.m_message, "~", "`~");
		std::vector<std::string> splitMessage = StringUtils::split(formattedMessage, "`");

		SetTextColor(message.m_color);
		printf("%s", formattedTime);

		for (auto& splStr : splitMessage) {
			if (splStr[0] == '~') {
				if (splStr.length() < 2) continue;
				switch (splStr[1]) {
					case '0': SetTextColor((int)ConsoleColor::WHITE); break;
					case '1': SetTextColor((int)ConsoleColor::GRAY); break;
					case '2': SetTextColor((int)ConsoleColor::DARKGRAY); break;
					case '3': SetTextColor((int)ConsoleColor::BLACK); break;
					case 'R': SetTextColor((int)ConsoleColor::DARKRED); break;
					case 'r': SetTextColor((int)ConsoleColor::RED); break;
					case 'Y': SetTextColor((int)ConsoleColor::DARKYELLOW); break;
					case 'y': SetTextColor((int)ConsoleColor::YELLOW); break;
					case 'G': SetTextColor((int)ConsoleColor::DARKGREEN); break;
					case 'g': SetTextColor((int)ConsoleColor::GREEN); break;
					case 'M': SetTextColor((int)ConsoleColor::DARKMAGENTA); break;
					case 'm': SetTextColor((int)ConsoleColor::MAGENTA); break;
					case 'C': SetTextColor((int)ConsoleColor::DARKCYAN); break;
					case 'c': SetTextColor((int)ConsoleColor::CYAN); break;
					case 'B': SetTextColor((int)ConsoleColor::DARKBLUE); break;
					case 'b': SetTextColor((int)ConsoleColor::BLUE); break;
					case 'X':
					case 'x':SetTextColor(message.m_color); break;
					default:SetTextColor(message.m_color); break;
				}
				splStr.erase(0, 2);
			}
			printf("%s", splStr.c_str());
			outputString += splStr;
		}
		printf("\n");

		char buffer2[0xffff] = { 0 };
		sprintf_s(buffer2, "%s%s %s\n", formattedTime, message.m_type.c_str(), outputString.c_str());
		LogToFile(buffer2);

		SetTextColor((int)ConsoleColor::WHITE);
	}

	void Logger::Message(int color, const char* type, const char* fmt, ...) {
		if (m_shuttingDown) return;
		while (m_queue.size() > MAXQUEUESIZE) {
			Sleep(0);
			//LOG_ERROR("[~2Logging~x] Console queue size exceeded 1000. Too many messages are being sent");
			//return;
		}
		char buffer[0xffff] = { 0 };
		va_list va_alist;

		va_start(va_alist, fmt);
		vsprintf_s(buffer, fmt, va_alist);
		va_end(va_alist);

		AddToQueue(color, buffer, type, time(nullptr));
	}

	void Logger::ForceEmptyQueue() {
		s_outputThread->shutdown();

		while (!s_outputThread->isFinished()) Sleep(0);

		while (!m_queue.empty()) {
			ProcessMessage(std::move(m_queue.front()));
			m_queue.pop();
		}
	}

	void Logger::LogToFile(const char* buff) {
		std::ofstream logFile;
		logFile.open(s_logPath, std::ios_base::app);
		logFile << buff;
	}

	void Logger::Cleanup() {
		if (!s_initialized) return;
		LOG("[~gLogging~x] deallocating console");
		ForceEmptyQueue();
		PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
	}
}