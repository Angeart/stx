#pragma once

#define stx_pp_cat(a,b) a##b
#define stx_cat(a,b) stx_pp_cat(a,b)
#define stx_pp_to_str(v) (#v)
#define stx_to_str(v) stx_pp_to_str(v)