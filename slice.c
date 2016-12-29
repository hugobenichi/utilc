#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
