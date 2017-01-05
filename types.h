#include <assert.h>

typedef float               f32;
typedef double              f64;

typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long int     i64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long int   u64;

#define _define_tuple_of(name, type)  struct name {\
                                        type x; \
                                        type y; \
                                      }
//_define_tuple_of(Tuple2_Of_F32, f32);

_define_tuple_of(Tuple2_Of_F32, f32);
//struct Tuple2_Of_F32 {
//  f32 x;
//  f32 y;
//};

struct Tuple3_Of_F32 {
  f32 x;
  f32 y;
  f32 z;
};

struct Tuple4_Of_F32 {
  f32 x;
  f32 y;
  f32 z;
  f32 t;
};

struct Tuple2_Of_F64 {
  f64 x;
  f64 y;
};

struct Tuple3_Of_F64 {
  f64 x;
  f64 y;
  f64 z;
};

struct Tuple4_Of_F64 {
  f64 x;
  f64 y;
  f64 z;
  f64 t;
};

// Complex numbers
typedef struct Tuple2_Of_F32 c64;
typedef struct Tuple2_Of_F64 c128;

// 2D & 3D Vectors
typedef struct Tuple2_Of_F32 v2_64;
typedef struct Tuple3_Of_F32 v3_96;
typedef struct Tuple4_Of_F32 v4_128;

typedef struct Tuple2_Of_F64 v2_128;
typedef struct Tuple3_Of_F64 v3_192;
typedef struct Tuple4_Of_F64 v4_256;

// Quaternions ...

// RGB pixels ...

// RGBA pixels ...

void geom_tests() {
  #define _assert_sizeof(t, s) assert(sizeof(t) == s)
  _assert_sizeof(i8, 1);
  _assert_sizeof(i16, 2);
  _assert_sizeof(i32, 4);
  _assert_sizeof(i64, 8);

  _assert_sizeof(u8, 1);
  _assert_sizeof(u16, 2);
  _assert_sizeof(u32, 4);
  _assert_sizeof(u64, 8);

  _assert_sizeof(f32, 4);
  _assert_sizeof(f64, 8);

  _assert_sizeof(c64, 8);
  _assert_sizeof(c128, 16);

  _assert_sizeof(v2_64, 8);
  _assert_sizeof(v3_96, 12);
  _assert_sizeof(v4_128, 16);

  _assert_sizeof(v2_128, 16);
  _assert_sizeof(v3_192, 24);
  _assert_sizeof(v4_256, 32);
  #undef _assert_sizeof
}
