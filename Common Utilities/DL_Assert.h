#ifndef DL_ASSERT_HEADER
#define DL_ASSERT_HEADER

#include "DL_Debug.h"
#include <crtdefs.h>

#ifdef  __cplusplus
extern "C" {
#endif

_CRTIMP void __cdecl _wassert(_In_z_ const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);

#ifdef  __cplusplus
}
#endif

#ifdef assert
#undef assert
#endif
#ifdef NDEBUG
#define assert(x) ((void)0)
#else


#define assert(_Expression) \
  if (! (_Expression)) \
  { \
	DL_ASSERT(#_Expression);\
  }
#endif

#ifdef _CRTBLD
#ifndef _ASSERT_OK
#error assert.h not for CRT internal use, use dbgint.h
#endif  /* _ASSERT_OK */
#include <cruntime.h>
#endif  /* _CRTBLD */

#include <crtdefs.h>

#undef  assert


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	_CRTIMP void __cdecl _wassert(_In_z_ const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )


//#define assert(MaxSize > myCurrentSize "Array is full! Can't add any more data.")assert(_Expression)     ((void)0)
#ifdef NDEBUG
#else  /* NDEBUG */



#endif  /* NDEBUG */


/*

#undef  assert

#ifdef  NDEBUG

#define assert(_Expression)     ((void)0)

#else

#ifdef  __cplusplus
extern "C" {
#endif

_CRTIMP void __cdecl _wassert(_In_z_ const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);

#ifdef  __cplusplus
}
#endif

#define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

#endif  /* NDEBUG */


#endif
