#pragma once

#if !defined(STX_BUILD) && defined(_MSC_VER)
#define STX_LIB_NAME_PREFIX "stx"
#if defined(_DEBUG)
#define STX_LIB_NAME_SUFFIX "_d"
#else
#define STX_LIB_NAME_SUFFIX 
#endif
#define STX_LIB_NAME_EXTENTION ".lib"

#pragma comment(lib,STX_LIB_NAME_PREFIX STX_LIB_NAME_SUFFIX STX_LIB_NAME_EXTENTION)
#endif
