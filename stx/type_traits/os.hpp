#pragma once
#include <boost/predef/os.h>

#include "integral_condition.hpp"
#include "../mpl/mpl.hpp"

#undef linux

namespace stx
{
	namespace os
	{
		namespace platforms
		{
			struct windows :
#if BOOST_OS_WINDOWS != 0
				std::true_type {};
#define os_windows
#else
				std::false_type {};
#endif
			struct mac :
#if BOOST_OS_MACOS != 0
				std::true_type {};
#define os_mac
#else
				std::false_type {};
#endif
			struct linux :
#if BOOST_OS_LINUX != 0
				std::true_type {};
#define os_linux
#else
				std::false_type {};
#endif
#if !(defined(os_windows) | defined(os_mac) | defined(os_linux))
#define os_unknown
#endif
			struct unknown : mpl::not_<mpl::or_<windows,mac,linux>>::type{};
		};
		using current_platform = mpl::if_<
			platforms::windows,
			platforms::windows,
			mpl::if_<
			platforms::mac,
			platforms::mac,
			mpl::if_<
			platforms::linux,
			platforms::linux,
			platforms::unknown
			>::type
			>::type
		>::type;
	}

}


