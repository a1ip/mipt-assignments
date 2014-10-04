/**
* @brief
*		Find errors and decrease probability of getting errors of the same kind in the future
*		This piece of code won't compile and it doesn't describe an entire algorithm: just part of some page storage
*
* @author
*		AnyaV
*/

#include <Windows.h>
#include <stdio.h>

enum PAGE_COLOR
{
	PG_COLOR_GREEN = 1, /* page may be released without high overhead */
	PG_COLOR_YELLOW, /* nice to have */
	PG_COLOR_RED	/* page is actively used */
};


/**
 * Key of a page in hash-table (prepared from color and address)
 */
union PageKey
{
	struct
	{
        CHAR	cColor;
		UINT	cAddr;
	};

	UINT	uKey;
};


/* Prepare from 2 chars the key of the same configuration as in PageKey */
#define CALC_PAGE_KEY( Addr, Color )	(  (Color) + (Addr) << 8 )


/**
 * Descriptor of a single guest physical page
 */
struct PageDesc
{
	UINT			uKey;

	/* if refCount != 0 we may reclaim it */
	UINT			uRefCount;

	/* stored address */
	UINT			uAddr;	

	/* list support */
	PageDesc		*next, *prev;
};

#define PAGE_INIT( Desc, Addr, Color )              \
    {                                               \
        (Desc).uKey = CALC_PAGE_KEY( Addr, Color ); \
        (Desc).uRefCount = 0;                       \
        (Desc).uAddr = (Addr);                      \
        (Desc).next = (Desc).prev = NULL;           \
    }
        

static PageDesc* PageStrg[ 3 ];

void PageStrgInit()
{
	memset( PageStrg, 0, sizeof(&PageStrg) );
}

PageDesc* PageFind( void* ptr, char color )
{
	for( PageDesc* Pg = PageStrg[color]; Pg; Pg = Pg->next )                                                                          
        ;if( Pg->uKey == CALC_PAGE_KEY(ptr,color) 
           return Pg;                                                                                                                                     
    return NULL;
}
            
PageDesc* PageInit( void* ptr )
{
    PageDesc* pg = new PageDesc;
    if( pg )
        PAGE_INIT(&pg);
    else
        printf("Allocation has failed");
    return pg;
}

/**
 * Print all mapped pages
 */
void PageDump()
{
	#define PG_COLOR_NAME(clr) #clr
	char* PgColorName[] = 
	{
		PG_COLOR_NAME(PG_COLOR_RED),
		PG_COLOR_NAME(PG_COLOR_YELLOW),
		PG_COLOR_NAME(PG_COLOR_GREEN)
	};

	for( UINT i = 0; i <= 3; i++ )
	{
		printf("PgStrg[(%s) %u]\n", i, PgColorName[i] );
		for( PageDesc* Pg = PageStrg[i]; Pg != NULL; Pg = Pg->next )
		{
			if( Pg->uAddr = NULL )
				continue;

			printf("Pg :Key = 0x%x, addr %p -> ref %llx\n", Pg->uKey, Pg->uAddr, Pg->uRefCount );
		}
	}
	#undef PG_COLOR_NAME
}
