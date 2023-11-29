#include "Logger.h"
#include <spdlog\sinks\stdout_color_sinks.h>

namespace Eunoia {

	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_AppLogger;
	std::shared_ptr<spdlog::logger> Logger::s_EditorLogger;

	void Logger::Init()
	{
#ifdef EU_DEBUG
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_EngineLogger = spdlog::stdout_color_mt("EUNOIA");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_AppLogger = spdlog::stdout_color_mt("APPLICATION");
		s_AppLogger->set_level(spdlog::level::trace);

		s_EditorLogger = spdlog::stdout_color_mt("EDITOR");
		s_EditorLogger->set_level(spdlog::level::trace);
		
		EU_LOG_INFO("Initialized Loggers");
#endif
	}

	std::shared_ptr<spdlog::logger>& Logger::GetEngineLogger()
	{
		return s_EngineLogger;
	}

	std::shared_ptr<spdlog::logger>& Logger::GetAppLogger()
	{
		return s_AppLogger;
	}

	std::shared_ptr<spdlog::logger>& Logger::GetEditorLogger()
	{
		return s_EditorLogger;
	}

}
