#pragma once
#include <type_traits>

namespace stx
{
	namespace mpl
	{
		template<bool, class B = std::true_type, class ... C>
		struct and_impl_
			: and_impl_<B::value, C...>
		{};

		template<>
		struct and_impl_<true, std::true_type>
			: std::true_type
		{};

		template<class B, class ... C>
		struct and_impl_<false, B, C...>
			: std::false_type
		{};

		template<class A, class ... B>
		struct and_
			: and_impl_<A::value, B...>
		{};


		template<bool, class B = std::false_type, class ... C>
		struct or_impl_
			: or_impl_<B::value, C...>
		{};

		template<>
		struct or_impl_<false, std::false_type>
			: std::false_type
		{};

		template<class B, class ... C>
		struct or_impl_<true, B, C...>
			: std::true_type
		{};
		template<class A, class ... B>
		struct or_
			: or_impl_<A::value, B...>
		{};

		template<class T>
		struct not_
		{
			using type = std::integral_constant<bool,!T::value>;
			static const bool value = !T::value;
		};

		template<bool C, class T1, class T2>
		struct if_c
		{
			using type = T1;
		};

		template<class T1, class T2>
		struct if_c<false, T1, T2>
		{
			using type = T2;
		};

		template<class T1, class T2, class T3>
		struct if_
			: if_c<T1::value, T2, T3>
		{};

		template<class T1, class T2, class T3>
		struct eval_if
			: if_<T1, T2, T3>::type
		{};

		template<class T>
		struct identity
		{
			using type = T;
		};
	}
}