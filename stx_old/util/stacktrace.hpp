#pragma once
#include "../util/flexcode.hpp"
#include <vector>

namespace stx
{
	namespace debug
	{
		stx::tstring backtrace(unsigned int trace_size = 10);
	}
}