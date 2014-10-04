//#include <windows.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

typedef unsigned int UINT;
typedef char CHAR;
typedef size_t SIZE_T;

enum PAGE_COLOR {
  PG_COLOR_GREEN = 1, // Page may be released without high overhead.
  PG_COLOR_YELLOW, // Nice to have.
  PG_COLOR_RED  // Page is actively used.
};

// Key of a page in hash-table (prepared from color and address).
struct PageKey {
  SIZE_T addr;
  PAGE_COLOR color;
};

// Descriptor of a single guest physical page.
struct PageDesc {
  PageKey key;
  UINT ref_count; // If refCount != 0 we may reclaim it.
  SIZE_T addr; // Stored address.
  PageDesc *next, *prev; // List support.
};

static PageDesc* PageStrg[3];

void PageStrgInit() {
  memset(PageStrg, 0, sizeof(PageStrg));
}

// Prepare from 2 chars the key of the same configuration as in PageKey.
PageKey CalcPageKey(SIZE_T addr, PAGE_COLOR color) {
  PageKey result;
  result.addr = addr;
  result.color = color;
  return result;
}

PageDesc* PageFind(void* ptr, PAGE_COLOR color) {
  for (PageDesc* page = PageStrg[0]; page; page = page->next) {
    if (page->key == CalcPageKey((SIZE_T)ptr, color)) {
      return page;
    }
  }
  return NULL;
}

#define PAGE_INIT(desc, address, color)           \
  do {                                            \
    (desc).key = CalcPageKey((address), (color)); \
    (desc).ref_count = 0;                         \
    (desc).addr = (address);                      \
    (desc).next = (desc).prev = NULL;             \
  } while(0)

PageDesc* PageInit(PAGE_COLOR color) {
  PageDesc* page = (PageDesc*)malloc(sizeof(PageDesc));
  if (page != NULL) {
    PAGE_INIT(*page, (SIZE_T)page, color);
  } else {
    printf("Allocation has failed");
  }
  return page;
}

// Print all mapped pages.
void PageDump() {
  #define PG_COLOR_NAME(clr) #clr

  const char* PgColorName[] = {
    PG_COLOR_NAME(PG_COLOR_GREEN),
    PG_COLOR_NAME(PG_COLOR_YELLOW),
    PG_COLOR_NAME(PG_COLOR_RED)
  };

  for (UINT i = 0; i < 3; i++) {
    printf("PgStrg[(%u) %s]\n", i, PgColorName[i]);
    for (PageDesc* page = PageStrg[i]; page != NULL; page = page->next) {
      if (page->addr == 0)
        continue;

      printf("Pg: key = 0x%x%x, addr = 0x%x, ref = 0x%x\n", page->key.addr, page->key.color, page->addr, page->ref_count);
    }
  }
  #undef PG_COLOR_NAME
}

int main() {
  return 0;
}
