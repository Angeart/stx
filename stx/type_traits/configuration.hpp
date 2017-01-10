#pragma once

#include "../mpl/mpl.hpp"
#include "integral_condition.hpp"

namespace stx
{
	namespace compiler
	{
		namespace configurations
		{
			struct release :
#if defined(_RELEASE) | defined(RELEASE)
				std::true_type {};
#else
				std::false_type {};
#endif
			struct debug :
#if defined(_DEBUG) | defined(DEBUG)
				std::true_type {};
#else
				std::false_type {};
#endif
			struct develop :
#if defined(_DEVELOP) | defined(DEVELOP)
				std::true_type {};
#else
				std::false_type {};
#endif
			struct test :
#if defined(_TEST) | defined(TEST)
				std::true_type {};
#else
				std::false_type {};
#endif
		}
		using current_configuration = mpl::if_<
			configurations::release,
			configurations::release,
			mpl::if_<
			configurations::debug,
			configurations::debug,
			mpl::if_<
			configurations::develop,
			configurations::develop,
			configurations::test
			>::type
			>::type
		>::type;
	}
}