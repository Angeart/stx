#pragma once
#include <functional>
#include "lambda_traits.hpp"


namespace stx
{
	namespace function_detail
	{
		namespace impl
		{
			template<class Null>
			struct to_function_impl
			{};

			template<class Rt, class ...Args>
			struct to_function_impl<Rt(Args...)>
			{
				static constexpr bool is_member_func = false;
				typedef std::function<Rt(Args...)> type;
				enum { arity = sizeof...(Args) };
				typedef void class_type;
			};

			template<class Rt, class Cls, class ...Args>
			struct to_function_impl<Rt(Cls::*)(Args...)> : to_function_impl<Rt(Args...)>
			{
				static constexpr bool is_member_func = true;
				typedef Cls class_type;
			};
		}

		template<class Ld>
		struct to_function : lambda_detail::lambda_type_inherit<Ld, impl::to_function_impl>
		{};

		template<class Rt, class ...Args>
		struct to_function<std::function<Rt(Args...)>> : impl::to_function_impl<Rt(Args...)>
		{};

		template<class Rt, class ...Args>
		struct to_function<std::function<Rt(*)(Args...)>> : impl::to_function_impl<Rt(Args...)>
		{};

		template<class Rt, class Cls, class ...Args>
		struct to_function<std::function<Rt(Cls::*)(Args...)>> : impl::to_function_impl<Rt(Cls::*)(Args...)>
		{};

		template<class Rt, class ...Args>
		struct to_function<Rt(Args...)> : impl::to_function_impl<Rt(Args...)>
		{};

		template<class Rt, class ...Args>
		struct to_function<Rt(*)(Args...)> : impl::to_function_impl<Rt(Args...)>
		{};

		template<class Rt, class Cls, class ...Args>
		struct to_function<Rt(Cls::*)(Args...)> : impl::to_function_impl<Rt(Cls::*)(Args...)>
		{};
	}

	template<class Fc>
	struct to_function :
		function_detail::to_function<Fc>
	{};
};