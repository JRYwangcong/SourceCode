#include <sys/types.h>  /* For size_t */
#include <stdio.h>

#define MFAIL       ((void *) -1)

// please define these symbols in linker script
extern unsigned char __heap[];
extern unsigned char __heap_end[];

static unsigned char * sbrk_top = __heap;

void *sbrk(unsigned int size);
void *sbrk(unsigned int size)
{
    void *ptr;

    // NOTE: we assume all allocated address are 4 byte-aligned!!!
    size = (size + 3) & (~3);

    if (size > 0)
    {
        if ((sbrk_top + size) <= __heap_end)
        {
            ptr = sbrk_top;
            sbrk_top += size;
            return ptr;
        }
    }
    else if (size == 0)
    {
        return sbrk_top;
    }
    else
    {
        printf("sbrk: error\n");
        // we don't currently support shrink behavior
    }
    return MFAIL;
}

void *_sbrk(unsigned int size) __attribute__ ((weak, alias("sbrk")));
