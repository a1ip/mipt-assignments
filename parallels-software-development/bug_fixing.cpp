#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

enum PAGE_COLOR {
  PG_COLOR_GREEN = 0, /* page may be released without high overhead */
  PG_COLOR_YELLOW, /* nice to have */
  PG_COLOR_RED  /* page is actively used */
};

/**
 * Key of a page in hash-table (prepared from color and address)
 */
union PageKey {
  struct {
    CHAR cColor;
    UINT cAddr;
  };

  UINT uKey;
};

/**
 * Descriptor of a single guest physical page
 */
struct PageDesc {
  UINT uKey;

  /* if refCount != 0 we may reclaim it */
  UINT uRefCount;

  /* stored address */
  UINT uAddr;  

  /* list support */
  PageDesc *next, *prev;
};

static PageDesc* PageStrg[3];

void PageStrgInit() {
  memset(PageStrg, 0, sizeof(PageStrg));
}

/* Prepare from 2 chars the key of the same configuration as in PageKey */
#define CALC_PAGE_KEY(addr, color) ((color) + ((addr) << 8))

PageDesc* PageFind(void* ptr, CHAR color) {
  for (PageDesc* page = PageStrg[color]; page != NULL; page = page->next) {
    if (page->uKey == CALC_PAGE_KEY((UINT)ptr, color)) {
       return page;
    }
  }
  return NULL;
}

#define PAGE_INIT(desc, addr, color)            \
  do {                                          \
    (desc).uKey = CALC_PAGE_KEY(addr, color);   \
    (desc).uRefCount = 0;                       \
    (desc).uAddr = (addr);                      \
    (desc).next = (desc).prev = NULL;           \
  } while(0)

PageDesc* PageInit(CHAR color) {
  PageDesc* page = (PageDesc*)malloc(sizeof(PageDesc));
  if (page) {
    PAGE_INIT(*page, (UINT)page, color);
  } else {
    fprintf(stderr, "Allocation has failed.\n");
  }
  return page;
}

/**
 * Print all mapped pages
 */
void PageDump() {
  #define PG_COLOR_NAME(clr) #clr

  const char* PgColorName[] = {
    PG_COLOR_NAME(PG_COLOR_GREEN),
    PG_COLOR_NAME(PG_COLOR_YELLOW),
    PG_COLOR_NAME(PG_COLOR_RED)
  };

  for (UINT i = 0; i < 3; i++) {
    printf("PgStrg[(%s) %u]\n", PgColorName[i], i);
    for (PageDesc* page = PageStrg[i]; page != NULL; page = page->next) {
      if (page->uAddr == 0) {
        continue;
      }

      printf("Page: key = 0x%x, addr = 0x%x, ref = %u\n", page->uKey, page->uAddr, page->uRefCount);
    }
  }
  #undef PG_COLOR_NAME
}
