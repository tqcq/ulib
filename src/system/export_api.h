//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_SYSTEM_EXPORTAPI_H_
#define ULIB_SRC_SYSTEM_EXPORTAPI_H_

#ifdef _WIN32
#ifdef ULIB_LIBRARY_IMPL
#define ULIB_EXPORT __declspec(dllexport)
#else
#define ULIB_EXPORT __declspec(dllimport)
#endif

#else // _WIN32

#if __has_attribute(visibility) && defined(ULIB_LIBRARY_IMPL)
#define ULIB_EXPORT __attribute__((visibility("default")))
#endif

#endif // _WIN32

#endif //ULIB_SRC_SYSTEM_EXPORTAPI_H_
