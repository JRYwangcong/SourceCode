#ifndef __MEMORY_MAP_SEL_H__
#define __MEMORY_MAP_SEL_H__

//------ SYSTEM MEMORY MAP ---------------------------------------------------------
#if defined(CONFIG_MSTAR_MST9U_MMAP_512MB)
    #include "mmap_512mb.h"
#elif defined(CONFIG_MSTAR_MST9U_MMAP_256MB)
    #include "mmap_256mb.h"
#else
    #error "Incorrect MMAP setting!!"
#endif

#endif /* __MEMORY_MAP_SEL_H__ */
