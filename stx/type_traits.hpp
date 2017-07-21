#pragma once
#include <xstddef>
#include <boost/mpl/or.hpp>
#include <type_traits>
#include <memory>

namespace stx {
    template<bool B, typename T = void> using enable_if = std::enable_if<B, T>;
    template<bool B, typename T = void> using enable_if_t = typename std::enable_if<B,T>::type;
	template<bool B, typename T = void> using disable_if = std::enable_if<!B, T>;
	template<bool B, typename T = void> using disable_if_t = typename disable_if<B, T>::type;

    // stop compile when _Ty is false
    template<class _Ty, enable_if_t<_Ty::value, std::nullptr_t> = nullptr>
    struct compile_error{};


}