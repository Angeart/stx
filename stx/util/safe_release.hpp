#pragma once
#include <type_traits>

#include "../type_traits.hpp"

namespace stx
{
	namespace util
	{
		template<class _Ty, typename std::enable_if_t<std::is_pointer<_Ty>::value> *& = enabler>
		void safe_release(_Ty& object)
		{
			if (object)
			{
				object->Release();
				object = nullptr;
			}
		}

		struct safe_releaser
		{
			template<class Any, typename std::enable_if_t<std::is_pointer<Any>::value> *& = enabler>
			safe_releaser& operator<< (Any r)
			{
				safe_release(r);
				return *this;
			}

		};

		static safe_releaser chain_release;
	}
}