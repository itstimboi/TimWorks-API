#ifndef TW_PLATFORM_DETECT_H
#define TW_PLATFORM_DETECT_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__EMSCRIPTEN__)
    #define TWAPI_PLATFORM_EMSCRIPTEN 1
#endif

#if defined(__wasi__)
    #define TWAPI_PLATFORM_WASI 1
#endif

/* ---------------------------------------------------------
   Mobile and specialized Unix-like platforms
   Check before Linux because some define Linux macros too.
   --------------------------------------------------------- */

#if defined(__ANDROID__)
    #define TWAPI_PLATFORM_ANDROID 1
#endif

#if defined(__OHOS__)
    #define TWAPI_PLATFORM_OPENHARMONY 1
#endif

#if defined(__Fuchsia__)
    #define TWAPI_PLATFORM_FUCHSIA 1
#endif

#if defined(__serenity__)
    #define TWAPI_PLATFORM_SERENITY 1
#endif

#if defined(__minix) || defined(__minix__)
    #define TWAPI_PLATFORM_MINIX 1
#endif

#if defined(__GNU__) && !defined(__FreeBSD_kernel__)
    #define TWAPI_PLATFORM_HURD 1
#endif

#if defined(__FreeBSD_kernel__) && !defined(__FreeBSD__)
    #define TWAPI_PLATFORM_GNU_KFREEBSD 1
#endif

#if defined(__MINT__)
    #define TWAPI_PLATFORM_MINT 1
#endif

/* ---------------------------------------------------------
   BSD operating systems
   --------------------------------------------------------- */

#if defined(__DragonFly__)
    #define TWAPI_PLATFORM_DRAGONFLY 1
    #define TWAPI_PLATFORM_BSD 1
#endif

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
    #define TWAPI_PLATFORM_FREEBSD 1
    #define TWAPI_PLATFORM_BSD 1
#endif

#if defined(__NetBSD__)
    #define TWAPI_PLATFORM_NETBSD 1
    #define TWAPI_PLATFORM_BSD 1
#endif

#if defined(__OpenBSD__)
    #define TWAPI_PLATFORM_OPENBSD 1
    #define TWAPI_PLATFORM_BSD 1
#endif

#if defined(bsdi) || defined(__bsdi) || defined(__bsdi__)
    #define TWAPI_PLATFORM_BSDI 1
    #define TWAPI_PLATFORM_BSD 1
#endif

/* ---------------------------------------------------------
   Other Unix and Unix-like systems
   --------------------------------------------------------- */

#if defined(_AIX)
    #define TWAPI_PLATFORM_AIX 1
#endif

#if defined(__HAIKU__)
    #define TWAPI_PLATFORM_HAIKU 1
#endif

#if defined(hpux) || defined(__hpux) || defined(__hpux__)
    #define TWAPI_PLATFORM_HPUX 1
#endif

#if defined(sgi) || defined(__sgi) || \
    defined(__sgi__) || defined(_SGI_SOURCE)
    #define TWAPI_PLATFORM_IRIX 1
#endif

#if defined(osf) || defined(__osf) || \
    defined(__osf__) || defined(_OSF_SOURCE)
    #define TWAPI_PLATFORM_OSF 1
#endif

#if defined(__QNXNTO__) || defined(__QNX__)
    #define TWAPI_PLATFORM_QNXNTO 1
#endif

#if defined(riscos) || defined(__riscos) || defined(__riscos__)
    #define TWAPI_PLATFORM_RISCOS 1
#endif

#if defined(__sun) && defined(__SVR4)
    #define TWAPI_PLATFORM_SOLARIS 1
#endif

#if defined(__CYGWIN__)
    #define TWAPI_PLATFORM_CYGWIN 1
#endif

/* ---------------------------------------------------------
   Linux family
   --------------------------------------------------------- */

#if defined(__linux__) || defined(__linux) || defined(linux)
    #define TWAPI_PLATFORM_LINUX 1
#endif

/* Linux-derived environments should remain distinct. */
#if defined(TWAPI_PLATFORM_ANDROID) || \
    defined(TWAPI_PLATFORM_OPENHARMONY)
    /* Keep the underlying Linux compiler macro available,
       but use the specific platform macro in API decisions. */
#endif

/* ---------------------------------------------------------
   Apple platforms
   --------------------------------------------------------- */

#if defined(__APPLE__) && defined(__MACH__)

    #define TWAPI_PLATFORM_APPLE 1

    #include <TargetConditionals.h>

    #if defined(TARGET_OS_MACCATALYST) && TARGET_OS_MACCATALYST
        #define TWAPI_PLATFORM_MACCATALYST 1
    #elif defined(TARGET_OS_VISION) && TARGET_OS_VISION
        #define TWAPI_PLATFORM_VISIONOS 1
    #elif defined(TARGET_OS_TV) && TARGET_OS_TV
        #define TWAPI_PLATFORM_TVOS 1
    #elif defined(TARGET_OS_WATCH) && TARGET_OS_WATCH
        #define TWAPI_PLATFORM_WATCHOS 1
    #elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
        #define TWAPI_PLATFORM_IOS 1
    #else
        #define TWAPI_PLATFORM_MACOS 1
    #endif

    #if defined(TARGET_OS_SIMULATOR) && TARGET_OS_SIMULATOR
        #define TWAPI_PLATFORM_APPLE_SIMULATOR 1
    #endif

#endif

/* ---------------------------------------------------------
   Windows family
   --------------------------------------------------------- */

#if defined(_WIN32) || defined(__CYGWIN__)

    #define TWAPI_PLATFORM_WINDOWS 1

    /*
       Cygwin provides a POSIX environment. Do not classify it
       as native desktop Win32 merely because _WIN32 is defined.
    */
    #if defined(__CYGWIN__)

        #define TWAPI_PLATFORM_CYGWIN 1

    #else

        /*
           Detect Windows Runtime API families when the SDK
           supplies winapifamily.h.
        */
        #if defined(__has_include)
            #if __has_include(<winapifamily.h>)
                #include <winapifamily.h>
                #define TWAPI_HAS_WINAPIFAMILY_H 1
            #endif
        #endif

        #ifndef TWAPI_HAS_WINAPIFAMILY_H
            #define TWAPI_HAS_WINAPIFAMILY_H 0
        #endif

        #if TWAPI_HAS_WINAPIFAMILY_H

            #if defined(WINAPI_FAMILY) && \
                defined(WINAPI_FAMILY_DESKTOP_APP) && \
                (WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP)

                #define TWAPI_PLATFORM_WINRT 1

                #if defined(WINAPI_FAMILY_APP) && \
                    (WINAPI_FAMILY == WINAPI_FAMILY_APP)
                    #define TWAPI_PLATFORM_UWP 1
                #endif

                #if defined(WINAPI_FAMILY_PHONE_APP) && \
                    (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
                    #define TWAPI_PLATFORM_WINDOWS_PHONE 1
                #endif

            #endif

        #endif

        /*
           Xbox and GDK targets.
           Check these before the ordinary desktop fallback.
        */
        #if defined(_GAMING_XBOX_XBOXONE)

            #define TWAPI_PLATFORM_XBOXONE 1

        #elif defined(_GAMING_XBOX_SCARLETT)

            #define TWAPI_PLATFORM_XBOXSERIES 1

        #elif defined(_GAMING_DESKTOP)

            #define TWAPI_PLATFORM_WINGDK 1

        #elif defined(TWAPI_PLATFORM_WINRT)

            /* Already classified as a Windows Runtime target. */

        #else

            #define TWAPI_PLATFORM_WIN32 1

        #endif

        #if defined(TWAPI_PLATFORM_WINGDK) || \
            defined(TWAPI_PLATFORM_XBOXONE) || \
            defined(TWAPI_PLATFORM_XBOXSERIES)
            #define TWAPI_PLATFORM_GDK 1
        #endif

    #endif

#endif

/* ---------------------------------------------------------
   Other legacy and console platforms
   --------------------------------------------------------- */

#if defined(__OS2__) || defined(__EMX__)
    #define TWAPI_PLATFORM_OS2 1
#endif

#if defined(__MSDOS__) || defined(MSDOS) || defined(_MSDOS)
    #define TWAPI_PLATFORM_DOS 1
#endif

#if defined(__PS2__) || defined(PS2)
    #define TWAPI_PLATFORM_PS2 1
#endif

#if defined(__PSP__) || defined(__psp__)
    #define TWAPI_PLATFORM_PSP 1
#endif

#if defined(__vita__) || defined(__psp2__)
    #define TWAPI_PLATFORM_VITA 1
#endif

#if defined(__3DS__)
    #define TWAPI_PLATFORM_3DS 1
#endif

#if defined(__NGAGE__)
    #define TWAPI_PLATFORM_NGAGE 1
#endif

#if defined(__MORPHOS__)
    #define TWAPI_PLATFORM_MORPHOS 1
#endif

#if defined(__AROS__)
    #define TWAPI_PLATFORM_AROS 1
#endif

#if defined(__amigaos4__) || defined(__AMIGAOS4__)
    #define TWAPI_PLATFORM_AMIGAOS4 1
#endif

#if defined(__amigaos__) || defined(__AMIGA__)
    #define TWAPI_PLATFORM_AMIGAOS 1
#endif

/* ---------------------------------------------------------
   Generic platform families
   --------------------------------------------------------- */

#if defined(__unix__) || defined(__unix) || defined(unix) || \
    defined(TWAPI_PLATFORM_CYGWIN) || \
    defined(TWAPI_PLATFORM_HAIKU) || \
    defined(TWAPI_PLATFORM_HURD) || \
    defined(TWAPI_PLATFORM_FREEBSD) || \
    defined(TWAPI_PLATFORM_NETBSD) || \
    defined(TWAPI_PLATFORM_OPENBSD) || \
    defined(TWAPI_PLATFORM_DRAGONFLY) || \
    defined(TWAPI_PLATFORM_AIX) || \
    defined(TWAPI_PLATFORM_HPUX) || \
    defined(TWAPI_PLATFORM_IRIX) || \
    defined(TWAPI_PLATFORM_OSF) || \
    defined(TWAPI_PLATFORM_QNXNTO) || \
    defined(TWAPI_PLATFORM_RISCOS) || \
    defined(TWAPI_PLATFORM_SOLARIS) || \
    defined(TWAPI_PLATFORM_MINIX) || \
    defined(TWAPI_PLATFORM_SERENITY)
    #define TWAPI_PLATFORM_UNIX 1
#endif

#ifdef __cplusplus
}
#endif

#endif