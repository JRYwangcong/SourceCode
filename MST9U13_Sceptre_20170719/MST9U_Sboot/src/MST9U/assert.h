#ifndef __PRANA_ASSERT_H__
#define __PRANA_ASSERT_H__

#ifdef NDEBUG
#define assert(ignore)  ((void) 0)
#else
#define assert(expr)    ((expr) ? (void)0 :  __assert(__FILE__, __LINE__, ""))
#endif

#ifndef ASSERT
#define ASSERT(expr)    ((expr) ? (void)0 : __assert(__FILE__, __LINE__, ""))
#endif

#ifdef __cplusplus
extern "C" {
#endif

void __assert(const char *file, int line, const char *expr);

#ifdef __cplusplus
}
#endif

#endif /* __PRANA_ASSERT_H__ */
