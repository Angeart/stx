#pragma once
#include <type_traits>

namespace stx
{
	template<class type_a, class type_b, class comp = bool,comp false_type = false>
	struct if_t
	{
		template<comp b>
		struct select
		{
			using type = type_a;
		};
		template <>
		struct select<false_type>
		{
			using type = type_b;
		};
	};

	template<class type_a, class type_b>
	using if_int_t = if_t<type_a, type_b, int, 0>;
}