#pragma once
#include "../config.hpp"
#include <boost/noncopyable.hpp>
#include <iostream>
#include <stack>

#include "flexcode.hpp"
#include "../types.h"

namespace stx
{
	enum class console_color : unsigned short
	{
		black = 0u,
		dark_blue,
		dark_green,
		dark_cyan,
		dark_red,
		dark_magenta,
		dark_yellow,
		dark_gray,
		gray,
		blue,
		green,
		cyan,
		red,
		magenta,
		yellow,
		white,
		clear = 0xffu,
	};

	namespace console_detail
	{
		class console
			: public boost::noncopyable
		{
		public:
			using self = console;

			console();
			~console() = default;

			void open();
			void close();

			self& operator[](const console_color& color);

			friend stx::tostream& operator<<(stx::tostream& os, const self& rhs);
		private:

			stx::generic_handle handle;

			std::stack<console_color> stacked_color;
		};
	}

	static console_detail::console console;
}
