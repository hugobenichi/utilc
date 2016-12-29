#include <stdlib.h>
#include <stdio.h>

// utilities for debugging malloc/free
//
// 1) declare a debug_malloc function that takes line and file info
// 2) use a macro to repoint malloc to debug_malloc with __LINE__ and __FILE__
// 3) trace calls to debug_malloc, debug_free and collect somewhere
// 4) regroup by pointer value and now it is trivial to do:
//      - leak analysis
//      - allocation profilling
//
// 5) also: - overallocate everythign and add a magic number at the end
//          - when freeing, scan memory and say if corrupted.

struct mem_debug_tracer {
    void (*tracer_malloc)(void* ptr, size_t size, const char* file, int line);
    char data[128];
};

struct mem_debug_tracer mem_debug_tracer = (struct mem_debug_tracer) {};

const /* i64 */ long mem_debug_magic_tag = 0xc64ebabe8badc0de;

void* (*true_malloc)(size_t) = malloc;

void* mem_debug_malloc(size_t size, int line, const char* file) {
  size_t header = 8;
  size_t footer = 8;
  size_t total = header + size + footer;

  void* p = true_malloc(total);
  if (!p) goto exit;

  *((long*) p) = total;
  *((long*)(p + header + size)) = mem_debug_magic_tag;

exit:
  if (mem_debug_tracer.tracer_malloc) {
    mem_debug_tracer.tracer_malloc(p, size, file, line);
  }
  return p;
}

void mem_debug_tracer_default_malloc(void* ptr, size_t size, const char* file, int line) {
  FILE* f = *((FILE**) &mem_debug_tracer.data);
  fprintf(f, "%p %s %ld %s %d\n", ptr, "malloc", size, file, line);
}

void mem_debug_tracer_set_default(FILE* f) {
  mem_debug_tracer = (struct mem_debug_tracer) {
      .tracer_malloc = mem_debug_tracer_default_malloc,
  };
  *((FILE**) &mem_debug_tracer.data) = f;
}

void mem_debug_tracer_clear() {
  mem_debug_tracer = (struct mem_debug_tracer) {};
}

#define malloc(s) mem_debug_malloc(s, __FILE__, __LINE__)

int main(int argc, char** args) {
  char* buf1 = malloc(10);

  mem_debug_tracer_set_default(stderr);

  char* buf2 = malloc(20);

  mem_debug_tracer_clear();

  char* buf3 = malloc(30);
}


// note for struct offset
//
// struct MyStruct {
//   int a
//   float b
//   long c
// };
//
// u32 b_offset = (u32)(((struct MyStruct*) 0) -> b);
