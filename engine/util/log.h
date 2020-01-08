#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <memory>
#include <optick.h>

namespace Rendy
{
	namespace Log
	{
		void init_logger();
		std::shared_ptr<spdlog::logger> get_logger();

		template<class T>
		void info(const T& msg)
		{	
			OPTICK_EVENT();
			get_logger()->info(msg);
		}

		template<typename... Args>
		void info(spdlog::string_view_t fmt, const Args&... args)
		{
			OPTICK_EVENT();
			get_logger()->info(fmt, args...);
		}

		template<class T>
		void warn(const T& msg)
		{
			OPTICK_EVENT();
			get_logger()->warn(msg);
		}

		template<typename... Args>
		void warn(spdlog::string_view_t fmt, const Args&... args)
		{
			OPTICK_EVENT();
			get_logger()->warn(fmt, args...);
		}

		template<class T>
		void error(const T& msg)
		{
			OPTICK_EVENT();
			get_logger()->error(msg);
		}

		template<typename... Args>
		void error(spdlog::string_view_t fmt, const Args&... args)
		{
			OPTICK_EVENT();
			get_logger()->error(fmt, args...);
		}
	};
};