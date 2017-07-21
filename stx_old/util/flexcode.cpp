#include "flexcode.hpp"

stx::tstring stx::to_tstring(const astring& str)
{
	using namespace flexcode;
	flex_converter<unicode_mode> converter;
	return std::move(converter.convert(str));
}

stx::astring stx::to_astring(const tstring& str)
{
	using namespace flexcode;
	flex_converter<unicode_mode> converter;
	return std::move(converter.revert(str));
}
