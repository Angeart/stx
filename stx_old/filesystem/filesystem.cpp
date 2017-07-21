#include "filesystem.hpp"
#include <boost/predef/os.h>
#include <boost/range/irange.hpp>

#if (BOOST_OS_WINDOWS)
#  include <stdlib.h>
#  include <windows.h>
#elif (BOOST_OS_SOLARIS)
#  include <stdlib.h>
#  include <limits.h>
#elif (BOOST_OS_LINUX)
#  include <unistd.h>
#  include <limits.h>
#elif (BOOST_OS_MACOS)
#  include <mach-o/dyld.h>
#elif (BOOST_OS_BSD_FREE)
#  include <sys/types.h>
#  include <sys/sysctl.h>
#endif

namespace stx
{
	namespace filesystem
	{

		path executable_path()
		{
#if (BOOST_OS_WINDOWS)
			tchar_t exePath[MAX_PATH];
			if (!GetModuleFileName(nullptr, exePath, MAX_PATH))
				exePath[0] = '\0';
#elif (BOOST_OS_SOLARIS)
			char exePath[PATH_MAX];
			if (realpath(getexecname(), exePath) == NULL)
				exePath[0] = '\0';
#elif (BOOST_OS_LINUX)
			char exePath[PATH_MAX];
			ssize_t len = ::readlink("/proc/self/exe", exePath, sizeof(exePath));
			if (len == -1 || len == sizeof(exePath))
				len = 0;
			exePath[len] = '\0';
#elif (BOOST_OS_MACOS)
			char exePath[PATH_MAX];
			uint32_t len = sizeof(exePath);
			if (_NSGetExecutablePath(exePath, &len) != 0) {
				exePath[0] = '\0'; // buffer too small (!)
			}
			else {
				// resolve symlinks, ., .. if possible
				char *canonicalPath = realpath(exePath, NULL);
				if (canonicalPath != NULL) {
					strncpy(exePath, canonicalPath, len);
					free(canonicalPath);
				}
			}
#elif (BOOST_OS_BSD_FREE)
			char exePath[2048];
			int mib[4];  mib[0] = CTL_KERN;  mib[1] = KERN_PROC;  mib[2] = KERN_PROC_PATHNAME;  mib[3] = -1;
			size_t len = sizeof(exePath);
			if (sysctl(mib, 4, exePath, &len, NULL, 0) != 0)
				exePath[0] = '\0';
#endif		
			return path(exePath);
		}

		path executable_dir()
		{
			auto path = executable_path();
			if (is_directory(path))
				return path;
			if (path.has_parent_path())
				return path.parent_path();
			return "";
		}
		line & operator >> (line & l, stx::tstring & str)
		{
			if (!l.ifs)
				std::logic_error("ifstream is null.");
			if (l.number < 1)
				std::invalid_argument("line number has be more than 1.");
			for (auto d : boost::irange(0u, l.number))
				std::getline(*l.ifs, str);
			return l;
		}

		line& operator >> (stx::tifstream & ifs, line & line)
		{
			line = line;
			line.ifs = &ifs;
			return line;
		}

		std::vector<line>& operator >> (stx::tifstream & ifs, std::vector<line>& lines)
		{
			for (auto& line : lines)
				ifs >> line;
			return lines;
		}

		std::vector<line>& operator >> (std::vector<line>& lines, std::vector<stx::tstring>& str)
		{
			if (lines.empty())
				return lines;

			std::sort(lines.begin(),lines.end(),
				[](const line& l, const line& r)->bool
			{
				return l.number < r.number;
			});

			stx::tstring buf;
			std::vector<stx::tstring> &result = str;

			auto it = lines.begin();
			auto distance = it->number;
			auto prev_end = lines.end() - 1;
			auto update_distance = [&]()
			{
				distance = (it + 1)->number - it->number;
			};
			auto get_line = [&]()
			{
				if (!it->ifs)
					std::logic_error("ifstream is null.");
				if (it->number < 1)
					std::invalid_argument("line number has be more than 1.");
				if (distance == 0)
				{
					update_distance();
					return;
				}
				for (auto d : boost::irange(0u, distance))
					std::getline(*it->ifs, buf);

				result.push_back(buf);
				update_distance();
				return;
			};

			if (lines.size() != 1)
			{
				for (; it != prev_end; ++it)
				{
					get_line();
				}
			}
			get_line();
			return lines;
		}
	}
}