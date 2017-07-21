#pragma once
#include "../type_traits.hpp"

namespace stx
{
	namespace union_cast_detail
	{
		template<class _to
				,class _from
				,typename std::enable_if<!std::is_reference<_to>::value>::type *& = stx::enabler>
		struct union_cast_t
		{
			union_cast_t(_from v)
				: value(v)
			{};
			_to cast() const { return casted; }
		private:
			union
			{
				_from value;
				_to casted;
			};
		};
	}

	template<class _to, class _from>
	_to union_cast(_from value)
	{
		return union_cast_detail::union_cast_t<_to, _from>(value).cast();
	}
}