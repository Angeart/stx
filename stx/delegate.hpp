#pragma once
#include <unordered_map>
#include <boost/get_pointer.hpp>
#include "type_traits/function_traits.hpp"
#include "type_traits/union_cast.hpp"
#include "mpl/mpl.hpp"
#include "util/console.hpp"

namespace stx
{
	template<class>
	class delegate{};
	template<class Ret, class ... Args>
	class delegate<Ret(Args...)>
	{
	public:
		using functor_type = typename to_function<Ret(Args...)>::type;
		using key_type = size_t;
		static constexpr key_type insert_failed = 0u;
	private:
		std::unordered_map<key_type,functor_type> functor;
	public:
		template<class origin_functor
				,class translator = stx::to_function<origin_functor>
				,class translated_functor_type = typename translator::type
				,typename std::enable_if<stx::mpl::and_<functor_type, translated_functor_type>::value>::type *& = stx::enabler>
		key_type operator+=(const origin_functor&& func)
		{
			//copy func to ref,when if func is rvalue reference
			auto ref = std::forward<origin_functor>(func);
			const auto key = stx::union_cast<key_type>(boost::get_pointer(ref));
			auto result = this->functor.emplace(key, ref);
			if (!result.second)
				return insert_failed;
			return key;
		}
		//can't remove rvalue reference functor
		template<class origin_functor
				,typename std::enable_if<!std::is_rvalue_reference<origin_functor>::value>::type *& = stx::enabler>
		void operator-=(const origin_functor&& func)
		{
			const auto key = stx::union_cast<key_type>(boost::get_pointer(func));
			auto it = this->functor.find(key);
			if (it != this->functor.end())
				this->functor.remove(it);
		}
		void operator()(Args... args) const
		{
			for (auto && item : functor) item(std::forward<Args>(args)...);
		}
	};
}
