
#ifndef IMMOLATE_API_H
#define IMMOLATE_API_H

#ifdef IMMOLATE_BUILT_AS_STATIC
#  define IMMOLATE_API
#  define IMMOLATE_NO_EXPORT
#else
#  ifndef IMMOLATE_API
#    ifdef Immolate_EXPORTS
        /* We are building this library */
#      define IMMOLATE_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define IMMOLATE_API __declspec(dllimport)
#    endif
#  endif

#  ifndef IMMOLATE_NO_EXPORT
#    define IMMOLATE_NO_EXPORT 
#  endif
#endif

#ifndef IMMOLATE_DEPRECATED
#  define IMMOLATE_DEPRECATED __declspec(deprecated)
#endif

#ifndef IMMOLATE_DEPRECATED_EXPORT
#  define IMMOLATE_DEPRECATED_EXPORT IMMOLATE_API IMMOLATE_DEPRECATED
#endif

#ifndef IMMOLATE_DEPRECATED_NO_EXPORT
#  define IMMOLATE_DEPRECATED_NO_EXPORT IMMOLATE_NO_EXPORT IMMOLATE_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef IMMOLATE_NO_DEPRECATED
#    define IMMOLATE_NO_DEPRECATED
#  endif
#endif

#endif /* IMMOLATE_API_H */
