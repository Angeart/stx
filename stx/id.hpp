#pragma once

#include <atomic>
#include <functional>

namespace stx {
    namespace id_detail {
        template<class _tag, class _type>
        struct id {
            using id_t = _type;
            static inline id generate() {
                static std::atomic<id_t> instance(0x0ul);
                return id{++instance};
            }
            id_t value;
        };
    }
    template<class _tag, class _type = unsigned long>
    using id = id_detail::id<_tag,_type>;
}

template<class _tag, class _type>
inline bool operator==(const stx::id<_tag, _type> &lhs, const stx::id<_tag, _type> &rhs) {
    return lhs.value == rhs.value;
}

template<class _tag, class _type>
inline bool operator!=(const stx::id<_tag, _type> &lhs, const stx::id<_tag, _type> &rhs) {
    return !(lhs.value == rhs.value);
}

template<class _tag, class _type>
inline bool operator<(const stx::id<_tag, _type> &lhs, const stx::id<_tag, _type> &rhs) {
    return lhs.value < rhs.value;
}

template<class _tag, class _type>
inline bool operator>(const stx::id<_tag, _type> &lhs, const stx::id<_tag, _type> &rhs) {
    return rhs.value < lhs.value;
}

namespace std {
    template<class _tag>
    struct hash<stx::id<_tag>>
    {
        std::size_t operator()(const stx::id<_tag> & key){
            return key.value;
        }
    };
}
