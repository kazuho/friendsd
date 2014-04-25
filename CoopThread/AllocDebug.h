/**
 * AllocDebug.h
 */

#ifndef ALLOCDEBUG_H
#define ALLOCDEBUG_H

#ifdef __cplusplus
extern "C" {
#endif
void* _AddPtr(void* p, const char* file, int line, const char* exp);
void* _ResetPtr(void* dst, void* src, const char* file, int line, const char* exp);
void* _RemovePtr(void* p, const char* file, int line, const char* exp);
int _AllocCount();
void _ShowAllocs();
#ifdef __cplusplus
}
#endif

/* macros of allocator, deallocators begins from here */

#ifdef __cplusplus

#ifdef ALLOCDEBUG
#define NEW(TYPE, OPT) ((TYPE*)(_AddPtr((new TYPE OPT), __FILE__, __LINE__, "new " #TYPE #OPT)))
#define NEW_ARRAY(TYPE, size) ((TYPE*)(_AddPtr((new TYPE [size]), __FILE__, __LINE__, "new " #TYPE "[" #size "]")))
#define DELETE(p) (_RemovePtr(p, __FILE__, __LINE__, "delete " #p), delete (p))
#define DELETE_ARRAY(p) (_RemovePtr(p, __FILE__, __LINE__, "delete [] " #p), delete [] (p))
#else
#define NEW(TYPE, OPT) (new TYPE OPT)
#define NEW_ARRAY(TYPE, size) (new TYPE [size])
#define DELETE(p) (delete (p))
#define DELETE_ARRAY(p) (delete [] p)
#endif

#endif

#ifdef ALLOCDEBUG
#define MALLOC(size) (_AddPtr(malloc(size), __FILE__, __LINE__, "malloc(" #size ")"))
#define REALLOC(p, size) (_ResetPtr(realloc(p, size), p, __FILE__, __LINE__, "realloc(" #p "," #size ")"))
#define FREE(p) (_RemovePtr(p, __FILE__, __LINE__, "free(" #p ")"), free(p))
#else
#define MALLOC(size) (malloc(size))
#define REALLOC(p, size) (realloc(p, size))
#define FREE(p) (free(p))
#endif

#endif
