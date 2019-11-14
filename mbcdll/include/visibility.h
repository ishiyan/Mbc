#pragma once
// Generic definitions for shared library

#if defined(_WIN32) || defined(__CYGWIN__)

#define DLL_LOCAL
#if defined(DLL_EXPORTS)
#define DLL_PUBLIC __declspec(dllexport)
#else
#define DLL_PUBLIC __declspec(dllimport)
#endif
//! The calling convention.
#define DLL_CC __cdecl

#elif defined(__GNUC__)

#define DLL_LOCAL  __attribute__ ((visibility("hidden")))
#if defined DLL_EXPORTS
#define DLL_PUBLIC __attribute__ ((visibility("default")))
#else
#define DLL_PUBLIC
#endif
//! The calling convention.
#define DLL_CC
//#define DLL_CC __attribute__((__cdecl__))

#else

#define DLL_LOCAL
#define DLL_PUBLIC
#error Unknown compiler

#endif
