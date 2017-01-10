#pragma once
#include <list>
#include <sstream>
#include <boost/filesystem.hpp>

#include "../type_traits/os.hpp"
#include "../type_traits/configuration.hpp"
#include "console.hpp"
#include "flexcode.hpp"
#include "singleton.hpp"

#ifdef os_windows
#include <windows.h>
#endif


namespace stx
{
	namespace logger_detail
	{
		enum class logger_mode
		{
			error,
			warnning,
			notification,
			information,
			reminder,
			debug
		};

		template<logger_mode flag>
		struct mode
		{
			const stx::tstring mode_str = "error";
		};

		template<>
		struct mode<logger_mode::warnning>
		{
			const stx::tstring mode_str = "warning";
		};

		template<>
		struct mode<logger_mode::notification>
		{
			const stx::tstring mode_str = "notification";
		};

		template<>
		struct mode<logger_mode::information>
		{
			const stx::tstring mode_str = "information";
		};

		template<>
		struct mode<logger_mode::reminder>
		{
			const stx::tstring mode_str = "reminder";
		};

		template<>
		struct mode<logger_mode::debug>
		{
			const stx::tstring mode_str = "debug";
		};


		using boost::system::error_code;
		namespace fs = boost::filesystem;

		class logger : public singleton<logger>
		{
		public:
			logger();
			~logger();

			stx::tofstream& get_stream() { return this->stream; }

		private:
			error_code folder_initialize() const;
			error_code open_logfile();
			error_code close_logfile();

			const fs::path folder_path = "./log/";
			fs::path logfile_path;

			stx::tofstream stream;
		};

		tstring get_time_string();


		template<logger_mode mode_flag, bool enable_log = true>
		class logger_stream
		{
			class logger_stream_buf_base : public stx::tstringbuf, public mode<mode_flag>
			{
			protected:
				std::list<stx::tostream*> streams;
			public:
				logger_stream_buf_base()
				{
					streams.emplace_back(&std::clog);
				}
				virtual int sync() override
				{
					for(auto* stream : this->streams)
					{
						*stream << str();
						stream->flush();
					}
					stx::statement::if_<stx::compiler::configurations::debug>([this]()
					{
#ifdef os_windows
						OutputDebugString(str().c_str());
#endif
					});
					str("");
					auto buffer = get_time_string();
					buffer += "[" + this->mode_str + "] ";
					sputn(buffer.c_str(), buffer.size());
					return 0;
				}
			};
			template<bool enable = true>
			class logger_stream_buf : public logger_stream_buf_base
			{
			public:
				logger_stream_buf(stx::tostream& stream)
				{
					this->streams.emplace_back(&stream);
				};
				logger_stream_buf(logger_stream_buf const& rhs)
				{
					this->streams = rhs.streams;
				};

			};

			template<>
			class logger_stream_buf<false> : public logger_stream_buf_base
			{
				logger_stream_buf(stx::tostream& stream) {};
				logger_stream_buf(logger_stream_buf const& rhs)
				{
					this->streams = rhs.streams;
				};
			};

			logger_stream_buf<enable_log> buffer;
		public:
			logger_stream(stx::tostream& stream) : buffer(stream)
			{
				buffer.sync();
			};
		};
	}

	static auto error()
	{
		using namespace logger_detail;
		return logger_stream<logger_mode::error>(logger::get_instance()->get_stream());
	}
}
