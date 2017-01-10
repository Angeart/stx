#pragma once
#include <type_traits>
#include <functional>

#include "../mpl/mpl.hpp"

namespace stx
{
	namespace statement
	{
		using delegate_type = std::function<void()>;

		template<typename condition>
		struct if_generator;

		template<typename condition>
		struct if_actor;

		template<typename generator>
		struct make_if_
		{
			using type = generator;
			static generator make(delegate_type&& functor)
			{
				return generator(std::forward<delegate_type&&>(functor));
			}
		};

		template<typename condition>
		struct if_actor
		{
			if_actor() {};
			if_actor(delegate_type&& functor) {};
		};

		template<>
		struct if_actor<std::true_type>
		{
			if_actor(delegate_type&& functor)
			{
				functor();
			};
		};

		template<typename inherit_condition>
		struct else_gen
		{
			template<typename internal_condition>
			typename make_if_<if_generator<typename mpl::and_<inherit_condition,internal_condition>::type>>::type else_if_(delegate_type&& functor)
			{
				return make_if_<if_generator<typename mpl::and_<inherit_condition, internal_condition>::type>>().make(std::forward<delegate_type&&>(functor));
			};

			typename make_if_<if_actor<typename mpl::not_<inherit_condition>::type>>::type else_(delegate_type&& functor) const
			{
				return make_if_<if_actor<typename mpl::not_<inherit_condition>::type>>().make(std::forward<delegate_type&&>(functor));
			}
		};

		template<typename condition>
		struct if_generator : if_actor<condition>, else_gen<condition>
		{
			if_generator() {};
			if_generator(delegate_type&& functor)
				: if_actor<condition>(std::forward<delegate_type&&>(functor))
			{};
		};

		template<typename condition>
		auto if_(delegate_type&& functor)
		{
			return if_generator<condition>(std::forward<delegate_type&&>(functor));
		}

	}
}