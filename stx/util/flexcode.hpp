#pragma once
#include "../config.hpp"
#include <locale>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <string>

#include "../type_traits/if_type.hpp"

namespace stx
{
	namespace flexcode
	{
		// detect unicode
#if defined(_UNICODE) | defined(UNICODE)
		using unicode_mode = std::true_type;
#else
		using unicode_mode = std::false_type;
#endif

		namespace felxcode_detail
		{
			using flexchar = if_t<wchar_t, char>;
		}
		using flexchar = felxcode_detail::flexchar::select<unicode_mode::value>;
		using anotherchar = felxcode_detail::flexchar::select<!unicode_mode::value>;

		namespace felxcode_detail
		{
			struct flex_converter_base
			{
			protected:
				std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			};
		}

		template<class unicode_mode = std::false_type>
		struct flex_converter : felxcode_detail::flex_converter_base
		{
			using from_type = std::wstring;
			using to_type = std::string;
			to_type&& convert(const from_type& str)
			{
				return std::move(converter.to_bytes(str));
			}
			from_type&& revert(const to_type& str)
			{
				return std::move(converter.from_bytes(str));
			}
		};

		template<>
		struct flex_converter<std::true_type> : felxcode_detail::flex_converter_base
		{
			using from_type = std::string;
			using to_type = std::wstring;
			to_type&& convert(const from_type& str)
			{
				return std::move(converter.from_bytes(str));
			}
			from_type&& revert(const to_type& str)
			{
				return std::move(converter.to_bytes(str));
			}
		};
	}

	using tstring = std::basic_string<flexcode::flexchar::type>;
	using astring = std::basic_string<flexcode::anotherchar::type>;
	using tchar_t = flexcode::flexchar::type;
	using achar_t = flexcode::anotherchar::type;

	using tiostream = std::basic_iostream<tchar_t>;
	using tostream = std::basic_ostream<tchar_t>;
	using tistream = std::basic_istream<tchar_t>;
	using tfstream = std::basic_fstream<tchar_t>;
	using tofstream = std::basic_ofstream<tchar_t>;
	using tifstream = std::basic_ifstream<tchar_t>;
	using tstringbuf = std::basic_stringbuf<stx::tchar_t>;

	tstring to_tstring(const astring& str);
	inline tstring to_tstring(const tstring& str) { return str; }
	astring to_astring(const tstring& str);
	inline astring to_astring(const astring& str) { return str; }

}
