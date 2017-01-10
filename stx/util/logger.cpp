#include "logger.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

stx::logger_detail::logger::logger()
{
	error_code error;

	stx::console.open();
	std::cout << stx::console[stx::console_color::white];

	if ((error = this->folder_initialize()))
		throw new std::runtime_error(error.message());
	if ((error = this->open_logfile()))
		throw new std::runtime_error(error.message());
}

stx::logger_detail::logger::~logger()
{
	error_code error;

	console.close();

	error = this->close_logfile();
	if (error)
		std::cout << error.message() << std::endl; // only show message
}

boost::system::error_code stx::logger_detail::logger::folder_initialize() const
{
	error_code error;

	if (fs::exists(this->folder_path))
	{
		if (fs::is_directory(this->folder_path))
		{
			if (!fs::remove((this->folder_path)))
				throw fs::filesystem_error("can not remove file!!", this->folder_path, error);
		}
	}
	else
	{
		if (!fs::create_directory(this->folder_path))
			throw fs::filesystem_error("can not create directory!!", this->folder_path, error);
	}

	return error;
}

namespace
{
	struct util
	{
		static stx::tstring get_log_filename()
		{
			using namespace boost::posix_time;
			ptime now = second_clock::local_time();
			stx::tstring date_string = stx::to_tstring(to_iso_extended_string(now));
			boost::algorithm::replace_all(date_string, ":", "_");
			return "log_" + date_string + ".txt";
		}
	};
}

boost::system::error_code stx::logger_detail::logger::open_logfile()
{
	error_code error;

	this->logfile_path = this->logfile_path / util::get_log_filename();

	this->stream.open(this->logfile_path.string<stx::tstring>());
	if (!stream)
		throw fs::filesystem_error("can not open log file!!", this->logfile_path, error);

	return error;
}

boost::system::error_code stx::logger_detail::logger::close_logfile()
{
	error_code error;

	if(stream)
	{
		stream.close();
		if (stream)
			throw fs::filesystem_error("can not close log file!!", this->logfile_path, error);
	}

	return error;
}

stx::tstring stx::logger_detail::get_time_string()
{
	using namespace boost::posix_time;
	ptime now = second_clock::local_time();
	return stx::to_tstring(to_iso_extended_string(now));
}
