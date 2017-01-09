#define _cat_impl(a,b) a##b
#define _cat(a,b) _cat_impl(a, b)

#define _addr_shift(ptr, offset) (void*)( (u8*)(ptr) + offset )

#define P(format, ...)  printf(#__VA_ARGS__ " = " #format "\n", __VA_ARGS__)
