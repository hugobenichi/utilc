#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


// Arrays: DOCME
struct array {
  int len; // TODO: should be unsigned
  int max;
  void* ptr; // TODO consider tagging the ptr last bit to indicate alignement ?
};

void array_borrow(struct array* a, void* ptr, int capacity) {
  a->len = 0;
  a->max = capacity;
  a->ptr = ptr;
}

// array_make allocate one block only, therefore
//   - should ptr go away and memory be appended right after ?
//   - or is there a way to allow both borrowed arrays (non-contiguous ptr) and owned arrays ?
struct array* array_make(int capacity) {
  struct array* a;
  size_t total_size = sizeof(*a) + capacity;
  a = malloc(total_size);
  if (a) {
    array_borrow(a, a + sizeof(*a), capacity);
  }
  return a;
}

void array_add_obj(struct array* a, void* obj, size_t obj_len) {
  size_t need = a->len + obj_len;
  if (need > a->max) {
    int new_max = a->len;
    while (new_max < need) {
      new_max *= 2;
    }
    a->ptr = realloc(a, new_max + sizeof(*a));
    if (a->ptr) {
        return;
    }
    a->max = new_max;
  }
  memcpy(a->ptr + a->len, obj, obj_len);
  a->len += obj_len;
}


#define _instantiate_array_getter(name, type) \
  type* name(struct array* a, int idx) { \
    type *p = (type*) a->ptr; \
    assert(idx * sizeof(*p) < a->len); \
    return p + idx; \
  }

_instantiate_array_getter(array_get_u8, u8);
_instantiate_array_getter(array_get_u16, u16);
_instantiate_array_getter(array_get_u32, u32);
_instantiate_array_getter(array_get_u64, u64);

// Slices: DOCME

struct slice {
  void* mem;
  int len;
  int max;
};

struct slice slice_make(int len) {
  return (struct slice) {
    .mem = malloc(len),
    .len = len,
    .max = len,
  };
}

struct slice slice_concat(struct slice s, void* obj, int obj_len) {
  int need = s.len + obj_len;
  if (need > s.max) {
    // TODO: how to use realloc ?
    //  - need to know how mem was alloced
    //  - or require parametrized allocator
    int new_max = s.len;
    while (new_max < need) {
      new_max *= 2;
    }
    void* new_mem = malloc(new_max);
    if (!new_mem) {
        // abort
    }
    memcpy(new_mem, s.mem, s.len);
    s.mem = new_mem;
    s.max = new_max;
  }
  memcpy(s.mem + s.len, obj, obj_len);
  s.len += obj_len;
  return s;
}

struct slice slice_concat_owned(struct slice s, void* obj, int obj_len) {
  void* mem = s.mem;
  s = slice_concat(s, obj, obj_len);
  if (mem != s.mem) free(mem);
  return s;
}

#define _slice_clear(s) s.len = 0

void example() {
    const char* bla = "bla";
    int bla_len = strlen(bla);
    int repeat = 10;

    struct slice s = slice_make(5); //repeat * bla_len + 1);
    s.len = 0;

    for (int i = 0; i < repeat; i++) {
        s = slice_concat_owned(s, (void*) bla, bla_len);
    }
    printf("s.len = %d\n", s.len);

    *(char *)(s.mem + s.len) = 0;

    printf("%s\n", s.mem);
}

int main(int argc, char** args) {
  example();
}
