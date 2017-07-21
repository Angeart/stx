#include "../type_traits/os.hpp"
#include "console.hpp"

#ifdef os_windows
#include <windows.h>
#pragma warning(push)
#pragma warning(disable:4996)
#endif
namespace stx
{
	namespace console_detail
	{
		using namespace stx::statement;
		using namespace stx::os::platforms;
		console::console() : handle(nullptr)
		{
		}
		void console::open()
		{
#ifdef os_windows
			if (AllocConsole() != 0)
			{
				this->handle = GetStdHandle(STD_OUTPUT_HANDLE);
				freopen("CONOUT$", "w", stdout);
				freopen("CONIN$", "r", stdin);
			}
			else
			{
				this->handle = GetStdHandle(STD_OUTPUT_HANDLE);
			}
#endif
		}
		void console::close()
		{
#ifdef os_windows
			if (this->handle)
			{
				FreeConsole();
				this->handle = nullptr;
			}
#endif
		}

		console::self & console::operator[](const console_color& color)
		{
#ifdef os_windows
			if (color != console_color::clear)
			{
				this->stacked_color.push(color);
				SetConsoleTextAttribute(this->handle, static_cast<unsigned short>(color));
			}
			else
			{
				if (!this->stacked_color.empty())
				{
					SetConsoleTextAttribute(this->handle, static_cast<unsigned short>(this->stacked_color.top()));
					this->stacked_color.pop();
				}
			}
#endif
			return *this;
		}
		stx::tostream & operator<<(stx::tostream & os, const console::self & rhs)
		{
#ifdef os_windows
			os << std::flush;
#endif
			return os;
		}
	}
}

#ifdef os_windows
#pragma warning(pop)
#endif
